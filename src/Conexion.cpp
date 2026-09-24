/**
 * @file Conexion.cpp
 * @brief Implementación de la clase Conexion.
 */

#include <thread>
#include <string>
#include <list>
#include <stdexcept>
#include <functional>
#include <nlohmann/json.hpp>
#include <sys/socket.h>
#include <iostream>
#include <algorithm>
#include "Mensaje.hpp"
#include "GeneraMensaje.hpp"
#include "GeneraJSON.hpp"
#include "ValidaMensaje.hpp"
#include "Conexion.hpp"

using json = nlohmann::json;

int Conexion::contadorId = 0;


Conexion::Conexion(int socketCliente){
  this->socketCliente = socketCliente;
  activa = true;
  id = ++contadorId;
  identificado = false;
}

Conexion::~Conexion(){
  desconecta();

  if(hilo.joinable()){
    hilo.join();
  }
}

void Conexion::iniciaHilo(){
  hilo = std::thread(&Conexion::recibeMensajes, this);
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

    auto ultimoSalto = buffer.find('\n');

    while(ultimoSalto != std::string::npos){
      std::string linea = buffer.substr(0, ultimoSalto);

      buffer.erase(0, ultimoSalto + 1);

      try{
	std::string sinEspacios = linea;

	sinEspacios.erase(
			  remove(sinEspacios.begin(), sinEspacios.end(), ' '),
			  sinEspacios.end()
			  );

	if(!sinEspacios.empty() && sinEspacios.back() == '\r')
	  sinEspacios.pop_back();

	if(sinEspacios.empty()){
	  ultimoSalto = buffer.find('\n');
	  continue;
	}

	json jsonMensaje = json::parse(linea);
	Mensaje mensaje = GeneraMensaje::genera(jsonMensaje);

	if(!ValidaMensaje::valida(mensaje)){
	  respuestaInvalida();
	  break;
	}

	for(std::function<void(Conexion&, const Mensaje&)> escucha : escuchas){
	  escucha(*this, mensaje);
	}

      }catch(const json::parse_error& pe){
	respuestaInvalida();
	break;

      }catch(const std::invalid_argument& ia){
	respuestaInvalida();
	break;
      }

      ultimoSalto = buffer.find('\n');
    }
  }
}

void Conexion::respuestaInvalida(){
  Mensaje mensajeInvalido = Mensaje::Builder()
    .setTipo(TipoMensaje::NO_VALIDO)
    .build();
  
  for(std::function<void(Conexion&, const Mensaje&)> escucha : escuchas)
    escucha(*this, mensajeInvalido);
}

void Conexion::enviaMensaje(const Mensaje& mensaje){
  json jsonMensaje = GeneraJSON::genera(mensaje);
  std::string lineaMensaje = jsonMensaje.dump() + "\n";

  std::cout << "[" << id << "] << " << lineaMensaje;

  std::size_t enviados = 0;
  while(enviados < lineaMensaje.size()){
    auto bytesEnviados = send(socketCliente, lineaMensaje.data()+enviados, lineaMensaje.size()-enviados, 0);

    if(bytesEnviados <= 0){
      activa = false;
      /**
      for(std::function<void(Conexion&, const Mensaje&)> escucha : escuchas){
	escucha(*this, Mensaje::Builder()
		.setTipo(TipoMensaje::DISCONNECT)
		.build());
      }
      */
      break;
    }
    enviados += bytesEnviados;
  }
}

void Conexion::agregaEscucha(std::function<void(Conexion&, const Mensaje&)> escucha){
  escuchas.push_back(escucha);
}

void Conexion::desconecta(){
  if(!activa)
    return;
  
  activa = false;
  shutdown(socketCliente, SHUT_RDWR);
  close(socketCliente);
}

bool Conexion::estaActiva(){
  return activa;
}
int Conexion::getId(){
  return id;
}

void Conexion::seIdentifico(){
  identificado = true;
}

bool Conexion::getIdentificado(){
  return identificado;
}

