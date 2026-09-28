/**
 * @file Cliente.cpp
 * @brief Implementación de la clase Cliente.
 */

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include "Usuario.hpp"
#include "Conexion.hpp"
#include "Cliente.hpp"
#include "TipoMensaje.hpp"
#include "EstatusUsuario.hpp"
#include "Mensaje.hpp"

Cliente::Cliente(std::string ipServidor, int puerto) :
  usuario("nombre"){
  int socketCliente = socket(AF_INET, SOCK_STREAM, 0);

  if(socketCliente < 0){
    std::cout << "No se pudo iniciar el cliente correctamente.\n";
    exit(1);
  }

  sockaddr_in direccion;
  direccion.sin_family = AF_INET;
  direccion.sin_addr.s_addr = inet_addr(ipServidor.c_str());
  direccion.sin_port = htons(puerto);

  if(connect(socketCliente, (sockaddr*)&direccion, sizeof(direccion)) < 0){
    std::cout << "No se pudo conectar al servidor.\n";
    close(socketCliente);
    exit(1);
  }

  conexion = std::make_unique<Conexion>(socketCliente);
  conexion->agregaEscucha([this](Conexion&, const Mensaje mensaje){
    mensajeRecibido(mensaje);
  });

  conexion->iniciaHilo();

  std::cout << "Conectado al servidor.\n";
}

void Cliente::identificarse(std::string nombre){
  usuario = Usuario(nombre);
  conexion->enviaMensaje(Mensaje::Builder()
			 .setTipo(TipoMensaje::IDENTIFY)
			 .setUsername(nombre)
			 .build());
}

void Cliente::cambiarEstatus(EstatusUsuario estatus){
  if(usuario.getEstatus() == estatus)
    return;

  usuario.setEstatus(estatus);

  conexion->enviaMensaje(Mensaje::Builder()
			 .setTipo(TipoMensaje::STATUS)
			 .setEstatus(estatus)
			 .build());
}

void Cliente::obtenerUsuarios(){
  conexion->enviaMensaje(Mensaje::Builder()
			 .setTipo(TipoMensaje::USERS)
			 .build());
}

void Cliente::enviarTexto(std::string usuario, std::string texto){
  conexion->enviaMensaje(Mensaje::Builder()
			 .setTipo(TipoMensaje::TEXT)
			 .setUsername(usuario)
			 .setText(texto)
			 .build());
}

void Cliente::enviarTextoPublico(std::string texto){
  conexion->enviaMensaje(Mensaje::Builder()
			 .setTipo(TipoMensaje::PUBLIC_TEXT)
			 .setText(texto)
			 .build());  
}

void Cliente::crearSala(std::string sala){
  conexion->enviaMensaje(Mensaje::Builder()
			 .setTipo(TipoMensaje::NEW_ROOM)
			 .setRoomname(sala)
			 .build());    
}

void Cliente::invitar(std::list<std::string> usuarios, std::string sala){
  conexion->enviaMensaje(Mensaje::Builder()
			 .setTipo(TipoMensaje::INVITE)
			 .setRoomname(sala)
			 .setUsernames(usuarios)
			 .build());    
}

void Cliente::entrarSala(std::string sala){
  conexion->enviaMensaje(Mensaje::Builder()
			 .setTipo(TipoMensaje::JOIN_ROOM)
			 .setRoomname(sala)
			 .build());      
}

void Cliente::obtenerUsuariosSala(std::string sala){
  conexion->enviaMensaje(Mensaje::Builder()
			 .setTipo(TipoMensaje::ROOM_USERS)
			 .setRoomname(sala)
			 .build());
}

void Cliente::enviarTextoSala(std::string sala, std::string texto){
  conexion->enviaMensaje(Mensaje::Builder()
			 .setTipo(TipoMensaje::ROOM_TEXT)
			 .setRoomname(sala)
			 .setText(texto)
			 .build());
}

void Cliente::salirSala(std::string sala){
  conexion->enviaMensaje(Mensaje::Builder()
			 .setTipo(TipoMensaje::LEAVE_ROOM)
			 .setRoomname(sala)
			 .build());  
}

void Cliente::desconectar(){
  if(!conexion->estaActiva())
    return;
  
  conexion->enviaMensaje(Mensaje::Builder()
			 .setTipo(TipoMensaje::DISCONNECT)
			 .build());

  conexion->desconecta();
}

void Cliente::mensajeRecibido(const Mensaje& mensaje){
  for(std::function<void(const Mensaje&)> escucha : escuchas)
    escucha(mensaje);
}

void Cliente::agregaEscucha(std::function<void(const Mensaje&)> escucha){
  escuchas.push_back(escucha);
}
