/**
 * @file GeneraJSON.cpp
 * @brief Implementación de la clase GeneraJSON.
 */

#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <list>
#include <stdexcept>
#include "TipoMensaje.hpp"
#include "OperacionMensaje.hpp"
#include "ResultadoMensaje.hpp"
#include "EstatusUsuario.hpp"
#include "Mensaje.hpp"
#include "CampoMensaje.hpp"
#include "GeneraJSON.hpp"

using json = nlohmann::json;

json GeneraJSON::generaTipo(const TipoMensaje& tipo){
  if(tipo == TipoMensaje::NO_VALIDO)
    throw std::invalid_argument("El tipo de mensaje no es valido");
  
  return tipo;
}

json GeneraJSON::generaOperacion(const OperacionMensaje& operacion){
  if(operacion == OperacionMensaje::NO_VALIDO)
    throw std::invalid_argument("La operación del mensaje no es valida");

  return operacion;
}

json GeneraJSON::generaResultado(const ResultadoMensaje& resultado){
  if(resultado == ResultadoMensaje::NO_VALIDO)
    throw std::invalid_argument("El resultado del mensaje no es valido");
  
  return resultado;
}

json GeneraJSON::generaEstatus(const EstatusUsuario& estatus){
  if(estatus == EstatusUsuario::NO_VALIDO)
    throw std::invalid_argument("El estatus del mensaje no es valido");

  return estatus;
}

json GeneraJSON::generaUsers(const std::unordered_map<std::string, EstatusUsuario>& users){
  json json;
  for(const auto& [username, estatus] : users){
    if(estatus == EstatusUsuario::NO_VALIDO)
      throw std::invalid_argument("Un estatus del mensaje no es valido");
    
    json[username] = estatus;
  }

  return json;
}

json GeneraJSON::generaUsernames(const std::list<std::string>& usernames){
  json json;
  for(const auto& username : usernames){
    json.push_back(username);
  }
  
  return json;
}

std::string GeneraJSON::obtieneUsername(const Mensaje& mensaje){
  if(!mensaje.getUsername().has_value())
    throw std::invalid_argument("El mensaje no contiene un username");

  return mensaje.getUsername().value();
}

std::string GeneraJSON::obtieneRoomname(const Mensaje& mensaje){
  if(!mensaje.getRoomname().has_value())
    throw std::invalid_argument("El mensaje no contiene roomname");

  return mensaje.getRoomname().value();
}

std::string GeneraJSON::obtieneText(const Mensaje& mensaje){
  if(!mensaje.getText().has_value())
    throw std::invalid_argument("El mensaje no contiene texto");

  return mensaje.getText().value();
}

json GeneraJSON::genera(const Mensaje& mensaje){
  json json;
  json[CampoMensaje::TYPE] = generaTipo(mensaje.getTipo());

  switch(mensaje.getTipo()){
  case TipoMensaje::IDENTIFY:
  case TipoMensaje::NEW_USER:
  case TipoMensaje::DISCONNECTED:
    json[CampoMensaje::USERNAME] = obtieneUsername(mensaje);
    break;

  case TipoMensaje::RESPONSE:{
    
    if(!mensaje.getOperacion().has_value() ||
       !mensaje.getResultado().has_value())
      throw std::invalid_argument("Al mensaje le hace falta su operación o resultado");

    OperacionMensaje operacion = mensaje.getOperacion().value();
    ResultadoMensaje resultado = mensaje.getResultado().value();

    json[CampoMensaje::OPERATION] = generaOperacion(operacion);
    json[CampoMensaje::RESULT] = generaResultado(resultado);

    if(operacion == OperacionMensaje::INVALID &&
       (resultado == ResultadoMensaje::NOT_IDENTIFIED ||
	resultado == ResultadoMensaje::INVALID))
      break;
    
    if(!mensaje.getExtra().has_value())
      throw std::invalid_argument("El mensaje no contiene extra");
    json[CampoMensaje::EXTRA] = mensaje.getExtra().value();
    break;
  }
    
  case TipoMensaje::STATUS:
    if(!mensaje.getEstatus().has_value())
      throw std::invalid_argument("El mensaje no contiene estatus");
    json[CampoMensaje::STATUS] = generaEstatus(mensaje.getEstatus().value());
    break;

  case TipoMensaje::NEW_STATUS:
    json[CampoMensaje::USERNAME] = obtieneUsername(mensaje);
    if(!mensaje.getEstatus().has_value())
      throw std::invalid_argument("El mensaje no contiene estatus");
    json[CampoMensaje::STATUS] = generaEstatus(mensaje.getEstatus().value());
    break;

  case TipoMensaje::USERS:
  case TipoMensaje::DISCONNECT:
    break;

  case TipoMensaje::USER_LIST:
    if(!mensaje.getUsers().has_value())
      throw std::invalid_argument("El mensaje no contiene la lista de usuarios");
    json[CampoMensaje::USERS] = generaUsers(mensaje.getUsers().value());
    break;

  case TipoMensaje::TEXT:
  case TipoMensaje::TEXT_FROM:
  case TipoMensaje::PUBLIC_TEXT_FROM:
    json[CampoMensaje::USERNAME] = obtieneUsername(mensaje);
    json[CampoMensaje::TEXT] = obtieneText(mensaje);
    break;

  case TipoMensaje::PUBLIC_TEXT:
    json[CampoMensaje::TEXT] = obtieneText(mensaje);
    break;

  case TipoMensaje::NEW_ROOM:
  case TipoMensaje::JOIN_ROOM:
  case TipoMensaje::ROOM_USERS:
  case TipoMensaje::LEAVE_ROOM:
    
    json[CampoMensaje::ROOMNAME] = obtieneRoomname(mensaje);
    break;

  case TipoMensaje::INVITE:
    json[CampoMensaje::ROOMNAME] = obtieneRoomname(mensaje);
    
    if(!mensaje.getUsernames().has_value())
      throw std::invalid_argument("El mensaje no contiene usernames");
    json[CampoMensaje::USERNAMES] = generaUsernames(mensaje.getUsernames().value());
    break;

  case TipoMensaje::INVITATION:
  case TipoMensaje::JOINED_ROOM:
  case TipoMensaje::LEFT_ROOM:
    
    json[CampoMensaje::USERNAME] = obtieneUsername(mensaje);
    json[CampoMensaje::ROOMNAME] = obtieneRoomname(mensaje);
    break;

  case TipoMensaje::ROOM_USER_LIST:
    json[CampoMensaje::ROOMNAME] = obtieneRoomname(mensaje);

    if(!mensaje.getUsers().has_value())
      throw std::invalid_argument("El mensaje no contiene la lista de usuarios");
    json[CampoMensaje::USERS] = generaUsers(mensaje.getUsers().value());
    break;

  case TipoMensaje::ROOM_TEXT:
    json[CampoMensaje::ROOMNAME] = obtieneRoomname(mensaje);
    json[CampoMensaje::TEXT] = obtieneText(mensaje);
    break;

  case TipoMensaje::ROOM_TEXT_FROM:
    json[CampoMensaje::ROOMNAME] = obtieneRoomname(mensaje);
    json[CampoMensaje::USERNAME] = obtieneUsername(mensaje);
    json[CampoMensaje::TEXT] = obtieneText(mensaje);
    break;

  default:
    throw std::invalid_argument("No se pudo generar el JSON");
  }

  return json;
}
