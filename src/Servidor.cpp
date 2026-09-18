/**
 * @file Servidor.cpp
 * @brief Implementación de la clase Servidor.
 */

#include <nlohmann/json.hpp>
#include <list>
#include <sys/socket.h>
#include <netinet/in.h>
#include <memory>
#include <cstdlib>
#include <print>
#include "Conexion.hpp"
#include "Mensaje.hpp"
#include "Usuario.hpp"
#include "TipoMensaje.hpp"
#include "OperacionMensaje.hpp"
#include "ResultadoMensaje.hpp"

Servidor::Servidor(int puerto){
  this->puerto = puerto;
  socketServidor = socket(AF_INET, SOCK_STREAM, 0);

  if(socketServidor == -1){
    std::print("No se pudo iniciar el servidor correctamente.");
    exit(1);
  }

  sockaddr_in direccion;
  direccion.sin_family = AF_INET;
  direccion.sin_addr.s_addr = INADDR_ANY;
  direccion.sin_port = htons(puerto);

  if(bind(socketServidor, (sockaddr*)&direccion, sizeof(direccion)) < 0){
    std::print("No se pudo asociar el socket al puerto.");
    exit(1);
  }

  if(listen(socketServidor, 5) < 0){
    std::print("No se pudo poner al socket a escuchar.");
    exit(1);
  }
}

void Servidor::sirve(){
  enEjecucion = true;
  std::print("Escuchando en el puerto {}.\n", puerto);

  while(enEjecucion)
    aceptaConexion();

  std::print("La ejecución del servidor termino.");
}

void Servidor::aceptaConexion(){
  int socketCliente = accept(socketServidor, nullptr, nullptr);

  if(socketCliente < 0){
    std::print("Error al recibir una conexion...");
    return;
  }

  std::unique_ptr<Conexion> conexion = std::make_unique<Conexion>(socketCliente, *this);
  conexiones.emplace_back(std::move(conexion));
  conexiones.back()->iniciaHilo();
}

void Servidor::mensajeRecibido(Conexion& conexion, const Mensaje& mensaje){
  std::lock_guard<std::mutex> lock(mutex);
  
  switch(mensaje.getTipo()){
  case TipoMensaje::IDENTIFY:{
    bool existe = false;
    std::string username = mensaje.getUsername().value();
    for(const std::unique_ptr<Conexion>& cnx : conexiones){
      if(cnx.get() != &conexion && cnx->estaIdentificado() &&
	 cnx->getUsuario()->getNombre() == username){
	conexion.enviaMensaje(Mensaje::Builder()
			      .setTipo(TipoMensaje::RESPONSE)
			      .setOperacion(OperacionMensaje::IDENTIFY)
			      .setResultado(ResultadoMensaje::USER_ALREADY_EXISTS)
			      .setExtra(username)
			      .build());
	existe = true;
	break;
      }
    }
    if(!existe){
      conexion.setUsuario(std::make_unique<Usuario>(username));
      usuarios.emplace(username, conexion.getUsuario()->getEstatus());
      conexion.enviaMensaje(Mensaje::Builder()
			    .setTipo(TipoMensaje::RESPONSE)
			    .setOperacion(OperacionMensaje::IDENTIFY)
			    .setResultado(ResultadoMensaje::SUCCESS)
			    .setExtra(username)
			    .build());
      
      for(const std::unique_ptr<Conexion>& cnx : conexiones){
	if(cnx.get() != &conexion && cnx->estaIdentificado()){
	  cnx->enviaMensaje(Mensaje::Builder()
			    .setTipo(TipoMensaje::NEW_USER)
			    .setUsername(username)
			    .build());
	}
      }
    }
    break;
  }
    
  case TipoMensaje::USERS:
    conexion.enviaMensaje(Mensaje::Builder()
			  .setTipo(TipoMensaje::USER_LIST)
			  .setUsers(usuarios)
			  .build());
    break;
    
  default:
    break;
  }
}

int main(){
  Servidor servidor(1234);
  servidor.sirve();
}
