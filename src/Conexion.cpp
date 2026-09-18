/**
 * @file Conexion.cpp
 * @brief Implementación de la clase Conexion.
 */

#include <memory>
#include <thread>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include "Usuario.hpp"
#include "Mensaje.hpp"
#include "GeneraMensaje.hpp"
#include "GeneraJSON.hpp"
#include "ValidaMensaje.hpp"
#include "Conexion.hpp"

using json = nlohmann::json;

Conexion::Conexion(int socketCliente, Servidor& servidor)
  : servidor(servidor){
  this->socketCliente = socketCliente;
  activa = true;
  usuario = nullptr;
}

Conexion::~Conexion(){
  if(hilo.joinable())
    hilo.join();

  close(socketCliente);
}

void Conexion::recibeMensajes(){
  char datos[TAMAÑO_MAXIMO];

  while(activa){
    auto bytesLeidos = read(socketCliente, datos, TAMAÑO_MAXIMO);

    if(bytesLeidos <= 0){
      activa = false;
      break;
    }

    buffer.append(datos, bytesLeidos);

    if(buffer.size() > TAMAÑO_MAXIMO &&
       buffer.find('\n') == std::string::npos){
      activa = false;
      break;
    }
    
    auto ultimoSalto = buffer.find('\n');
    
    while(ultimoSalto != std::string::npos){
      std::string linea = buffer.substr(0, ultimoSalto);

      if(linea.size() > TAMAÑO_MAXIMO){
	activa = false;
	break;
      }
      
      buffer.erase(0, ultimoSalto+1);

      try{
	json jsonMensaje = json::parse(linea);
	Mensaje mensaje = GeneraMensaje::genera(jsonMensaje);
	if(!ValidaMensaje::valida(mensaje))
	  throw std::invalid_argument("Mensaje no valido");
	servidor.mensajeRecibido(*this, mensaje);
      }catch(const json::parse_error& pe){
	
      }catch(const std::invalid_argument& ia){
	
      }
      
      ultimoSalto = buffer.find('\n');
    }
  }
  close(socketCliente);
}

void Conexion::enviaMensaje(const Mensaje& mensaje){
  json jsonMensaje = GeneraJSON::genera(mensaje);
  std::string lineaMensaje = jsonMensaje.dump() + "\n";

  std::size_t enviados = 0;
  while(enviados < lineaMensaje.size()){
    auto bytesEnviados = send(socketCliente, lineaMensaje.data()+enviados, lineaMensaje.size()-enviados, 0);

    if(bytesEnviados <= 0){
      activa = false;
      break;
    }

    enviados += bytesEnviados;
  }
}

bool Conexion::estaIdentificado(){
  return usuario != nullptr;
}

void Conexion::iniciaHilo(){
  hilo = std::thread(&Conexion::recibeMensajes, this);
}

const std::unique_ptr<Usuario>& Conexion::getUsuario() const{
  return usuario;
}

void Conexion::setUsuario(std::unique_ptr<Usuario> usuario){
  this->usuario = std::move(usuario);
}
