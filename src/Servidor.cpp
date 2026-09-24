/**
 * @file Servidor.cpp
 * @brief Implementación de la clase Servidor.
 */

#include <nlohmann/json.hpp>
#include <list>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <memory>
#include <cstdlib>
#include <iostream>
#include "Conexion.hpp"
#include "Mensaje.hpp"
#include "Usuario.hpp"
#include "TipoMensaje.hpp"
#include "OperacionMensaje.hpp"
#include "ResultadoMensaje.hpp"
#include "EstatusUsuario.hpp"
#include "GeneraJSON.hpp"
#include "Servidor.hpp"

Servidor::Servidor(int puerto){
  this->puerto = puerto;
  socketServidor = socket(AF_INET, SOCK_STREAM, 0);

  if(socketServidor == -1){
    std::cout << "No se pudo iniciar el servidor correctamente.\n";
    exit(1);
  }

  int opcion = 1;
  if(setsockopt(socketServidor, SOL_SOCKET, SO_REUSEADDR, &opcion, sizeof(opcion)) < 0){
    std::cout << "No se pudo iniciar el servidor correctamente.\n";
    close(socketServidor);
    exit(1);
  }
  
  sockaddr_in direccion;
  direccion.sin_family = AF_INET;
  direccion.sin_addr.s_addr = INADDR_ANY;
  direccion.sin_port = htons(puerto);

  if(bind(socketServidor, (sockaddr*)&direccion, sizeof(direccion)) < 0){
    std::cout << "No se pudo asociar el socket al puerto.\n";
    exit(1);
  }

  if(listen(socketServidor, 5) < 0){
    std::cout << "No se pudo poner al socket a escuchar.\n";
    exit(1);
  }
}

void Servidor::sirve(){
  enEjecucion = true;
  std::cout << "Escuchando en el puerto " << puerto << ".\n";
  
  while(enEjecucion)
    aceptaConexion();
  
  std::cout << "La ejecución del servidor termino.\n";
}

void Servidor::aceptaConexion(){
  int socketCliente = accept(socketServidor, nullptr, nullptr);

  if(socketCliente < 0){
    std::cout << "Error al recibir una conexion...\n";
    return;
  }

  std::lock_guard<std::mutex> lock(mutex);
  
  ConexionUsuario cnxUsuario;
  cnxUsuario.conexion = std::make_unique<Conexion>(socketCliente);
  conexiones.emplace_back(std::move(cnxUsuario));
  conexiones.back().conexion->agregaEscucha([this](Conexion& conexion, const Mensaje& mensaje) {
    mensajeRecibido(conexion, mensaje);
  });
  idsConectadas.insert(conexiones.back().conexion->getId());
  conexiones.back().conexion->iniciaHilo();
}

void Servidor::mensajeRecibido(Conexion& conexion, const Mensaje& mensaje){
  std::lock_guard<std::mutex> lock(mutex);

  if(!conexion.estaActiva() && idsConectadas.contains(conexion.getId())){
    desconecta(conexion);
  }

  std::string username;

  try{
    std::string lineaMensaje = GeneraJSON::genera(mensaje).dump();
    std::cout << "[" << conexion.getId() << "] >> " << lineaMensaje << '\n';
  }catch(const std::invalid_argument& ia){
    std::cout << "[" << conexion.getId() << "] >> Mensaje no valido.\n";
  }
  
  for(ConexionUsuario& cnxUsuario : conexiones){
    if(cnxUsuario.conexion.get() == &conexion){
      username = cnxUsuario.username;
      break;
    }
  }
  
  if(!usuariosIdentificados.contains(username) && mensaje.getTipo() != TipoMensaje::IDENTIFY){
    conexion.enviaMensaje(Mensaje::Builder()
			  .setTipo(TipoMensaje::RESPONSE)
			  .setOperacion(OperacionMensaje::INVALID)
			  .setResultado(ResultadoMensaje::NOT_IDENTIFIED)
			  .build());
    return;
  }
  
  switch(mensaje.getTipo()){
  case TipoMensaje::IDENTIFY:
    identifica(conexion, mensaje);
    break;

  case TipoMensaje::STATUS:
    estatus(conexion, mensaje);
    break;
    
  case TipoMensaje::USERS:
    conexion.enviaMensaje(Mensaje::Builder()
			  .setTipo(TipoMensaje::USER_LIST)
			  .setUsers(usuariosIdentificados)
			  .build());
    break;

  case TipoMensaje::TEXT:
    texto(conexion, mensaje);
    break;

  case TipoMensaje::PUBLIC_TEXT:
    textoPublico(conexion, mensaje);
    break;

  case TipoMensaje::NEW_ROOM:
    nuevaSala(conexion, mensaje);
    break;

  case TipoMensaje::INVITE:
    invita(conexion, mensaje);
    break;

  case TipoMensaje::JOIN_ROOM:
    entrarSala(conexion, mensaje);
    break;

  case TipoMensaje::ROOM_USERS:
    usuariosSala(conexion, mensaje);
    break;

  case TipoMensaje::ROOM_TEXT:
    textoSala(conexion, mensaje);
    break;

  case TipoMensaje::LEAVE_ROOM:
    salirSala(conexion, mensaje);
    break;

  case TipoMensaje::DISCONNECT:
    desconecta(conexion);
    break;
    
  default:
    conexion.enviaMensaje(Mensaje::Builder()
			  .setTipo(TipoMensaje::RESPONSE)
			  .setOperacion(OperacionMensaje::INVALID)
			  .setResultado(ResultadoMensaje::INVALID)
			  .build());
    
    desconecta(conexion);
    break;
  }
}

void Servidor::identifica(Conexion& conexion, const Mensaje& mensaje){
  if(conexion.getIdentificado())
    return;
  
  std::string username = mensaje.getUsername().value();

  if(usuariosIdentificados.contains(username)){
    conexion.enviaMensaje(Mensaje::Builder()
			  .setTipo(TipoMensaje::RESPONSE)
			  .setOperacion(OperacionMensaje::IDENTIFY)
			  .setResultado(ResultadoMensaje::USER_ALREADY_EXISTS)
			  .setExtra(username)
			  .build());

    return;
  }
 
  usuariosIdentificados.emplace(username, EstatusUsuario::ACTIVE);
  for(ConexionUsuario& cnxUsuario : conexiones){
    if(cnxUsuario.conexion.get() == &conexion){
      cnxUsuario.username = username;
      break;
    }
  }
      
  conexion.enviaMensaje(Mensaje::Builder()
			.setTipo(TipoMensaje::RESPONSE)
			.setOperacion(OperacionMensaje::IDENTIFY)
			.setResultado(ResultadoMensaje::SUCCESS)
			.setExtra(username)
			.build());

  conexion.seIdentifico();
  
  for(const ConexionUsuario& cnxUsuario : conexiones){
    if(cnxUsuario.conexion.get() != &conexion &&
       usuariosIdentificados.contains(cnxUsuario.username) &&
       idsConectadas.contains(cnxUsuario.conexion->getId())){
      cnxUsuario.conexion->enviaMensaje(Mensaje::Builder()
					      .setTipo(TipoMensaje::NEW_USER)
					      .setUsername(username)
					      .build());
    }
  }
}

void Servidor::estatus(Conexion& conexion, const Mensaje& mensaje){
  std::string username;
  EstatusUsuario nuevoEstatus = mensaje.getEstatus().value();
  
  for(ConexionUsuario& cnxUsuario : conexiones){
    if(cnxUsuario.conexion.get() == &conexion){
      username = cnxUsuario.username;
      break;
    }
  }
  
  usuariosIdentificados[username] = nuevoEstatus;

  Mensaje cambioEstatus = Mensaje::Builder()
    .setTipo(TipoMensaje::NEW_STATUS)
    .setUsername(username)
    .setEstatus(nuevoEstatus)
    .build();

  for(const ConexionUsuario& cnxUsuario : conexiones){
    if(cnxUsuario.conexion.get() != &conexion &&
       usuariosIdentificados.contains(cnxUsuario.username) &&
       idsConectadas.contains(cnxUsuario.conexion->getId())){
      cnxUsuario.conexion->enviaMensaje(cambioEstatus);
    }
  }
}

void Servidor::texto(Conexion& conexion, const Mensaje& mensaje){
  std::string receptor = mensaje.getUsername().value();
  if(!usuariosIdentificados.contains(receptor)){
    conexion.enviaMensaje(Mensaje::Builder()
			  .setTipo(TipoMensaje::RESPONSE)
			  .setOperacion(OperacionMensaje::TEXT)
			  .setResultado(ResultadoMensaje::NO_SUCH_USER)
			  .setExtra(receptor)
			  .build());
    return;
  }

  std::string mensajero;
  
  for(ConexionUsuario& cnxUsuario : conexiones){
    if(cnxUsuario.conexion.get() == &conexion){
      mensajero = cnxUsuario.username;
      break;
    }
  }
  
  for(const ConexionUsuario& cnxUsuario : conexiones){
    if(cnxUsuario.username == receptor &&
       idsConectadas.contains(cnxUsuario.conexion->getId())){
      cnxUsuario.conexion.get()->enviaMensaje(Mensaje::Builder()
					      .setTipo(TipoMensaje::TEXT_FROM)
					      .setUsername(mensajero)
					      .setText(mensaje.getText().value())
					      .build());

      break;
    }
  }
}

void Servidor::textoPublico(Conexion& conexion, const Mensaje& mensaje){
  std::string mensajero;
  
  for(ConexionUsuario& cnxUsuario : conexiones){
    if(cnxUsuario.conexion.get() == &conexion){
      mensajero = cnxUsuario.username;
      break;
    }
  }

  for(const ConexionUsuario& cnxUsuario : conexiones){
    if(cnxUsuario.conexion.get() != &conexion &&
       usuariosIdentificados.contains(cnxUsuario.username) &&
       idsConectadas.contains(cnxUsuario.conexion->getId())){
      cnxUsuario.conexion.get()->enviaMensaje(Mensaje::Builder()
					      .setTipo(TipoMensaje::PUBLIC_TEXT_FROM)
					      .setUsername(mensajero)
					      .setText(mensaje.getText().value())
					      .build());
    }
  }
}

void Servidor::nuevaSala(Conexion& conexion, const Mensaje& mensaje){
  std::string roomname = mensaje.getRoomname().value();

  if(salas.contains(roomname)){
    conexion.enviaMensaje(Mensaje::Builder()
			  .setTipo(TipoMensaje::RESPONSE)
			  .setOperacion(OperacionMensaje::NEW_ROOM)
			  .setResultado(ResultadoMensaje::ROOM_ALREADY_EXISTS)
			  .setExtra(roomname)
			  .build());
    return;
  }
  
  Sala sala;
  sala.nombre = roomname;
  
  for(ConexionUsuario& cnxUsuario : conexiones){
    if(cnxUsuario.conexion.get() == &conexion){
      sala.miembros[cnxUsuario.username] = cnxUsuario.conexion.get();
      sala.usuariosEnSala[cnxUsuario.username] = usuariosIdentificados[cnxUsuario.username];
      break;
    }
  }

  salas[roomname] = sala;

  conexion.enviaMensaje(Mensaje::Builder()
			.setTipo(TipoMensaje::RESPONSE)
			.setOperacion(OperacionMensaje::NEW_ROOM)
			.setResultado(ResultadoMensaje::SUCCESS)
			.setExtra(roomname)
			.build());
}

void Servidor::invita(Conexion& conexion, const Mensaje& mensaje){
  std::string roomname = mensaje.getRoomname().value();

  if(!salas.contains(roomname)){
    conexion.enviaMensaje(Mensaje::Builder()
			  .setTipo(TipoMensaje::RESPONSE)
			  .setOperacion(OperacionMensaje::INVITE)
			  .setResultado(ResultadoMensaje::NO_SUCH_ROOM)
			  .setExtra(roomname)
			  .build());
    return;
  }
  
  std::string usuario;

  for(ConexionUsuario& cnxUsuario : conexiones){
    if(cnxUsuario.conexion.get() == &conexion){
      usuario = cnxUsuario.username;
      break;
    }
  }

  if(!salas[roomname].miembros.contains(usuario)){
    return;
  }

  std::list<std::string> nombres = mensaje.getUsernames().value();
  
  for(std::string nombre : nombres)
    if(!usuariosIdentificados.contains(nombre)){
      conexion.enviaMensaje(Mensaje::Builder()
			    .setTipo(TipoMensaje::RESPONSE)
			    .setOperacion(OperacionMensaje::INVITE)
			    .setResultado(ResultadoMensaje::NO_SUCH_USER)
			    .setExtra(nombre)
			    .build());
      return;
    }

  for(std::string nombre : nombres){
    if(salas[roomname].miembros.contains(nombre) ||
       salas[roomname].invitados.contains(nombre))
      continue;
    
    for(ConexionUsuario& cnxUsuario : conexiones){
      if(cnxUsuario.username == nombre &&
	 idsConectadas.contains(cnxUsuario.conexion->getId())){
	cnxUsuario.conexion.get()->enviaMensaje(Mensaje::Builder()
						.setTipo(TipoMensaje::INVITATION)
						.setUsername(usuario)
						.setRoomname(roomname)
						.build());
	
	salas[roomname].invitados.insert(nombre);
	break;
      }
    }
  }
}

void Servidor::entrarSala(Conexion& conexion, const Mensaje& mensaje){
  std::string roomname = mensaje.getRoomname().value();
  std::string usuario;
  
  for(ConexionUsuario& cnxUsuario : conexiones){
    if(cnxUsuario.conexion.get() == &conexion){
      usuario = cnxUsuario.username;
      break;
    }
  }
  
  if(!salas.contains(roomname)){
    conexion.enviaMensaje(Mensaje::Builder()
			  .setTipo(TipoMensaje::RESPONSE)
			  .setOperacion(OperacionMensaje::JOIN_ROOM)
			  .setResultado(ResultadoMensaje::NO_SUCH_ROOM)
			  .setExtra(roomname)
			  .build());
    return;
  }

  if(!salas[roomname].invitados.contains(usuario)){
    conexion.enviaMensaje(Mensaje::Builder()
			  .setTipo(TipoMensaje::RESPONSE)
			  .setOperacion(OperacionMensaje::JOIN_ROOM)
			  .setResultado(ResultadoMensaje::NOT_INVITED)
			  .setExtra(roomname)
			  .build());
    return;
  }

  salas[roomname].miembros[usuario] = &conexion;
  salas[roomname].invitados.erase(usuario);
  salas[roomname].usuariosEnSala[usuario] = usuariosIdentificados[usuario];
  conexion.enviaMensaje(Mensaje::Builder()
			.setTipo(TipoMensaje::RESPONSE)
			.setOperacion(OperacionMensaje::JOIN_ROOM)
			.setResultado(ResultadoMensaje::SUCCESS)
			.setExtra(roomname)
			.build());

  for(const auto& [nombre, cnx] : salas[roomname].miembros){
    if(cnx != &conexion)
      cnx->enviaMensaje(Mensaje::Builder()
			.setTipo(TipoMensaje::JOINED_ROOM)
			.setRoomname(roomname)
			.setUsername(usuario)
			.build());
  }
}

void Servidor::usuariosSala(Conexion& conexion, const Mensaje& mensaje){
  std::string roomname = mensaje.getRoomname().value();
  std::string usuario;
  
  for(ConexionUsuario& cnxUsuario : conexiones){
    if(cnxUsuario.conexion.get() == &conexion){
      usuario = cnxUsuario.username;
      break;
    }
  }
  
  if(!salas.contains(roomname)){
    conexion.enviaMensaje(Mensaje::Builder()
			  .setTipo(TipoMensaje::RESPONSE)
			  .setOperacion(OperacionMensaje::ROOM_USERS)
			  .setResultado(ResultadoMensaje::NO_SUCH_ROOM)
			  .setExtra(roomname)
			  .build());
    return;
  }

  if(!salas[roomname].miembros.contains(usuario)){
    conexion.enviaMensaje(Mensaje::Builder()
			  .setTipo(TipoMensaje::RESPONSE)
			  .setOperacion(OperacionMensaje::ROOM_USERS)
			  .setResultado(ResultadoMensaje::NOT_JOINED)
			  .setExtra(roomname)
			  .build());
    return;
  }

  conexion.enviaMensaje(Mensaje::Builder()
			.setTipo(TipoMensaje::ROOM_USER_LIST)
			.setRoomname(roomname)
			.setUsers(salas[roomname].usuariosEnSala)
			.build());
}

void Servidor::textoSala(Conexion& conexion, const Mensaje& mensaje){
  std::string roomname = mensaje.getRoomname().value();
  std::string usuario;
  
  for(ConexionUsuario& cnxUsuario : conexiones){
    if(cnxUsuario.conexion.get() == &conexion){
      usuario = cnxUsuario.username;
      break;
    }
  }
  
  if(!salas.contains(roomname)){
    conexion.enviaMensaje(Mensaje::Builder()
			  .setTipo(TipoMensaje::RESPONSE)
			  .setOperacion(OperacionMensaje::ROOM_TEXT)
			  .setResultado(ResultadoMensaje::NO_SUCH_ROOM)
			  .setExtra(roomname)
			  .build());
    return;
  }

  if(!salas[roomname].miembros.contains(usuario)){
    conexion.enviaMensaje(Mensaje::Builder()
			  .setTipo(TipoMensaje::RESPONSE)
			  .setOperacion(OperacionMensaje::ROOM_TEXT)
			  .setResultado(ResultadoMensaje::NOT_JOINED)
			  .setExtra(roomname)
			  .build());
    return;
  }
  
  for(const auto& [nombre, cnx] : salas[roomname].miembros){
    if(cnx != &conexion)
      cnx->enviaMensaje(Mensaje::Builder()
		       .setTipo(TipoMensaje::ROOM_TEXT_FROM)
		       .setRoomname(roomname)
		       .setUsername(usuario)
		       .setText(mensaje.getText().value())
		       .build());
  }
}

void Servidor::salirSala(Conexion& conexion, const Mensaje& mensaje){
  std::string roomname = mensaje.getRoomname().value();
  std::string usuario;
  
  for(ConexionUsuario& cnxUsuario : conexiones){
    if(cnxUsuario.conexion.get() == &conexion){
      usuario = cnxUsuario.username;
      break;
    }
  }
  
  if(!salas.contains(roomname)){
    conexion.enviaMensaje(Mensaje::Builder()
			  .setTipo(TipoMensaje::RESPONSE)
			  .setOperacion(OperacionMensaje::LEAVE_ROOM)
			  .setResultado(ResultadoMensaje::NO_SUCH_ROOM)
			  .setExtra(roomname)
			  .build());
    return;
  }

  if(!salas[roomname].miembros.contains(usuario)){
    conexion.enviaMensaje(Mensaje::Builder()
			  .setTipo(TipoMensaje::RESPONSE)
			  .setOperacion(OperacionMensaje::LEAVE_ROOM)
			  .setResultado(ResultadoMensaje::NOT_JOINED)
			  .setExtra(roomname)
			  .build());
    return;
  }

  salas[roomname].miembros.erase(usuario);
  salas[roomname].usuariosEnSala.erase(usuario);
  
  for(const auto& [nombre, cnx] : salas[roomname].miembros){
    cnx->enviaMensaje(Mensaje::Builder()
		      .setTipo(TipoMensaje::LEFT_ROOM)
		      .setRoomname(roomname)
		      .setUsername(usuario)
		      .build());
  }
}

void Servidor::desconecta(Conexion& conexion){
  std::string usuario;

  for(ConexionUsuario& cnxUsuario : conexiones){
    if(cnxUsuario.conexion.get() == &conexion){
      usuario = cnxUsuario.username;
      break;
    }
  }

  usuariosIdentificados.erase(usuario);
  idsConectadas.erase(conexion.getId());
  
  for(auto& [roomname, sala] : salas){
    if(sala.miembros.contains(usuario)){
      sala.miembros.erase(usuario);
      sala.usuariosEnSala.erase(usuario);
      for(const auto& [nombre, cnx] : sala.miembros){
	cnx->enviaMensaje(Mensaje::Builder()
			  .setTipo(TipoMensaje::LEFT_ROOM)
			  .setRoomname(roomname)
			  .setUsername(usuario)
			  .build());
      }
    }

    if(sala.invitados.contains(usuario)){
      sala.invitados.erase(usuario);
    }
  }
  
  for(ConexionUsuario& cnxUsuario : conexiones){
    if(usuariosIdentificados.contains(cnxUsuario.username) &&
       idsConectadas.contains(cnxUsuario.conexion->getId())){
      cnxUsuario.conexion.get()->enviaMensaje(Mensaje::Builder()
					      .setTipo(TipoMensaje::DISCONNECTED)
					      .setUsername(usuario)
					      .build());
    }
  }
  
  conexion.desconecta();
}

void Servidor::detenerServidor(){
  if(!enEjecucion)
    return;
  
  enEjecucion = false;
  shutdown(socketServidor, SHUT_RDWR);
  close(socketServidor);

  {
    std::lock_guard<std::mutex> lock(mutex);
    
    for(ConexionUsuario& cnxUsuario : conexiones){
      cnxUsuario.conexion->desconecta();
    }
    
  }
  
}

Servidor::~Servidor(){
  detenerServidor();
}
