/**
 * @file GeneraMensaje.cpp
 * @brief Implementación de la clase GeneraMensaje.
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
#include "GeneraMensaje.hpp"

using json = nlohmann::json;

TipoMensaje GeneraMensaje::generaTipo(const json& json){
  if(!json.contains(CampoMensaje::TYPE))
    throw std::invalid_argument("El mensaje no contiene un tipo");

  TipoMensaje tipo = json[CampoMensaje::TYPE].get<TipoMensaje>();

  if(tipo == TipoMensaje::NO_VALIDO)
    throw std::invalid_argument("Tipo de mensaje no valido");
  
  return tipo;
}

OperacionMensaje GeneraMensaje::generaOperacion(const json& json){
  if(!json.contains(CampoMensaje::OPERATION))
    throw std::invalid_argument("El mensaje no contiene una operación");

  OperacionMensaje operacion = json[CampoMensaje::OPERATION].get<OperacionMensaje>();

  if(operacion == OperacionMensaje::NO_VALIDO)
    throw std::invalid_argument("Operacion del mensaje no valida");

  return operacion;
}

ResultadoMensaje GeneraMensaje::generaResultado(const json& json){
  if(!json.contains(CampoMensaje::RESULT))
    throw std::invalid_argument("El mensaje no contiene un resultado");

  ResultadoMensaje resultado = json[CampoMensaje::RESULT].get<ResultadoMensaje>();

  if(resultado == ResultadoMensaje::NO_VALIDO)
    throw std::invalid_argument("Resultado del mensaje no valido");

  return resultado;
}

std::string GeneraMensaje::generaUsername(const json& json){
  if(!json.contains(CampoMensaje::USERNAME))
    throw std::invalid_argument("El mensaje no contiene username");
  
  return json[CampoMensaje::USERNAME].get<std::string>();
}

std::string GeneraMensaje::generaExtra(const json& json){
  if(!json.contains(CampoMensaje::EXTRA))
    throw std::invalid_argument("El mensaje no contiene extra");
  
  return json[CampoMensaje::EXTRA].get<std::string>();
}

EstatusUsuario GeneraMensaje::generaEstatus(const json& json){
  if(!json.contains(CampoMensaje::STATUS))
    throw std::invalid_argument("El mensaje no contiene estatus");

  EstatusUsuario estatus = json[CampoMensaje::STATUS].get<EstatusUsuario>();

  if(estatus == EstatusUsuario::NO_VALIDO)
    throw std::invalid_argument("Estatus del mensaje no valido");

  return estatus;
}

std::unordered_map<std::string, EstatusUsuario> GeneraMensaje::generaUsers(const json& json){
  if(!json.contains(CampoMensaje::USERS))
    throw std::invalid_argument("El mensaje no contiene una lista de usuarios");

  std::unordered_map<std::string, EstatusUsuario> users;

  for(const auto& [username, status] : json[CampoMensaje::USERS].items()){
    EstatusUsuario estatus = status.get<EstatusUsuario>();

    if(estatus == EstatusUsuario::NO_VALIDO)
      throw std::invalid_argument("Estatus dentro del mensaje no valido");

    users[username] = estatus;
  }

  return users;
}

std::string GeneraMensaje::generaText(const json& json){
  if(!json.contains(CampoMensaje::TEXT))
    throw std::invalid_argument("El mensaje no contiene texto");

  return json[CampoMensaje::TEXT].get<std::string>();
}

std::string GeneraMensaje::generaRoomname(const json& json){
  if(!json.contains(CampoMensaje::ROOMNAME))
    throw std::invalid_argument("El mensaje no contiene roomname");

  return json[CampoMensaje::ROOMNAME].get<std::string>();
}

std::list<std::string> GeneraMensaje::generaUsernames(const json& json){
  if(!json.contains(CampoMensaje::USERNAMES))
    throw std::invalid_argument("El mensaje no contiene un usernames");

  return json[CampoMensaje::USERNAMES].get<std::list<std::string>>();  
}

Mensaje GeneraMensaje::genera(const json& json){
  TipoMensaje tipo = GeneraMensaje::generaTipo(json);

  switch(tipo){
  case TipoMensaje::IDENTIFY:
  case TipoMensaje::NEW_USER:
  case TipoMensaje::DISCONNECTED:
    return Mensaje::Builder()
      .setTipo(tipo)
      .setUsername(GeneraMensaje::generaUsername(json))
      .build();

  case TipoMensaje::RESPONSE: {
    OperacionMensaje operacion = GeneraMensaje::generaOperacion(json);
    ResultadoMensaje resultado = GeneraMensaje::generaResultado(json);
    
    Mensaje::Builder builder = Mensaje::Builder()
      .setTipo(tipo)
      .setOperacion(operacion)
      .setResultado(resultado);
    
    if(operacion == OperacionMensaje::INVALID &&
       (resultado == ResultadoMensaje::NOT_IDENTIFIED ||
	resultado == ResultadoMensaje::INVALID))
      return builder.build();
    
    return builder.setExtra(GeneraMensaje::generaExtra(json))
      .build();
  }

  case TipoMensaje::STATUS:
    return Mensaje::Builder()
      .setTipo(tipo)
      .setEstatus(GeneraMensaje::generaEstatus(json))
      .build();

  case TipoMensaje::NEW_STATUS:
    return Mensaje::Builder()
      .setTipo(tipo)
      .setUsername(GeneraMensaje::generaUsername(json))
      .setEstatus(GeneraMensaje::generaEstatus(json))
      .build();

  case TipoMensaje::USERS:
  case TipoMensaje::DISCONNECT:
    return Mensaje::Builder()
      .setTipo(tipo)
      .build();

  case TipoMensaje::USER_LIST:
    return Mensaje::Builder()
      .setTipo(tipo)
      .setUsers(GeneraMensaje::generaUsers(json))
      .build();

  case TipoMensaje::TEXT:
  case TipoMensaje::TEXT_FROM:
  case TipoMensaje::PUBLIC_TEXT_FROM:
    return Mensaje::Builder()
      .setTipo(tipo)
      .setUsername(GeneraMensaje::generaUsername(json))
      .setText(GeneraMensaje::generaText(json))
      .build();

  case TipoMensaje::PUBLIC_TEXT:
    return Mensaje::Builder()
      .setTipo(tipo)
      .setText(GeneraMensaje::generaText(json))
      .build();

  case TipoMensaje::NEW_ROOM:
  case TipoMensaje::JOIN_ROOM:
  case TipoMensaje::ROOM_USERS:
  case TipoMensaje::LEAVE_ROOM:
    return Mensaje::Builder()
      .setTipo(tipo)
      .setRoomname(GeneraMensaje::generaRoomname(json))
      .build();

  case TipoMensaje::INVITE:
    return Mensaje::Builder()
      .setTipo(tipo)
      .setRoomname(GeneraMensaje::generaRoomname(json))
      .setUsernames(GeneraMensaje::generaUsernames(json))
      .build();

  case TipoMensaje::INVITATION:
  case TipoMensaje::JOINED_ROOM:
  case TipoMensaje::LEFT_ROOM:
    return Mensaje::Builder()
      .setTipo(tipo)
      .setUsername(GeneraMensaje::generaUsername(json))
      .setRoomname(GeneraMensaje::generaRoomname(json))
      .build();

  case TipoMensaje::ROOM_USER_LIST:
    return Mensaje::Builder()
      .setTipo(tipo)
      .setRoomname(GeneraMensaje::generaRoomname(json))
      .setUsers(GeneraMensaje::generaUsers(json))
      .build();

  case TipoMensaje::ROOM_TEXT:
    return Mensaje::Builder()
      .setTipo(tipo)
      .setRoomname(GeneraMensaje::generaRoomname(json))
      .setText(GeneraMensaje::generaText(json))
      .build();

  case TipoMensaje::ROOM_TEXT_FROM:
    return Mensaje::Builder()
      .setTipo(tipo)
      .setRoomname(GeneraMensaje::generaRoomname(json))
      .setUsername(GeneraMensaje::generaUsername(json))
      .setText(GeneraMensaje::generaText(json))
      .build();

  default:
    throw std::invalid_argument("No se pudo generar el mensaje");
  }

  throw std::invalid_argument("No se pudo generar el mensaje");
}
