/**
 * @file ValidaMensaje.cpp
 * @brief Implementación de la clase ValidaMensaje.
 */

#include <string>
#include <optional>
#include <list>
#include <unordered_map>
#include "Mensaje.hpp"
#include "Usuario.hpp"
#include "EstatusUsuario.hpp"
#include "TipoMensaje.hpp"
#include "OperacionMensaje.hpp"
#include "ResultadoMensaje.hpp"
#include "ValidaMensaje.hpp"

bool ValidaMensaje::validaIdentify(const Mensaje& mensaje){
  if(!mensaje.getUsername().has_value())
    return false;
  
  std::string nombre = mensaje.getUsername().value();

  if(nombre.empty() || nombre.length() > 8)
    return false;

  return true;
}

bool ValidaMensaje::validaResponse(const Mensaje& mensaje){
  if(!mensaje.getOperacion().has_value() ||
     !mensaje.getResultado().has_value())
    return false;
  
  OperacionMensaje operacion = mensaje.getOperacion().value();
  ResultadoMensaje resultado = mensaje.getResultado().value();
  std::string extra;

  if(mensaje.getExtra().has_value())
    extra = mensaje.getExtra().value();

  switch(operacion){
  case OperacionMensaje::IDENTIFY:
    if((resultado == ResultadoMensaje::SUCCESS ||
	resultado == ResultadoMensaje::USER_ALREADY_EXISTS) &&
       !extra.empty())
      return true;
    
    return false;

  case OperacionMensaje::TEXT:
    if(resultado == ResultadoMensaje::NO_SUCH_USER && !extra.empty())
      return true;

    return false;

  case OperacionMensaje::NEW_ROOM:
    if((resultado == ResultadoMensaje::SUCCESS ||
	resultado == ResultadoMensaje::ROOM_ALREADY_EXISTS) &&
       !extra.empty())
      return true;

    return false;

  case OperacionMensaje::INVITE:
    if((resultado == ResultadoMensaje::NO_SUCH_ROOM ||
	resultado == ResultadoMensaje::NO_SUCH_USER) &&
       !extra.empty())
      return true;

    return false;

  case OperacionMensaje::JOIN_ROOM:
    if((resultado == ResultadoMensaje::SUCCESS ||
	resultado == ResultadoMensaje::NO_SUCH_ROOM ||
	resultado == ResultadoMensaje::NOT_INVITED) &&
       !extra.empty())
      return true;

    return false;

  case OperacionMensaje::ROOM_USERS:
  case OperacionMensaje::ROOM_TEXT:
  case OperacionMensaje::LEAVE_ROOM:
    if((resultado == ResultadoMensaje::NO_SUCH_ROOM ||
	resultado == ResultadoMensaje::NOT_JOINED) &&
       !extra.empty())
      return true;
    
    return false;

  case OperacionMensaje::INVALID:
    if(resultado == ResultadoMensaje::NOT_IDENTIFIED ||
       resultado == ResultadoMensaje::INVALID)
      return true;
    
    return false;

  default:
    return false;
  }

  return false;
}

bool ValidaMensaje::validaNewUser(const Mensaje& mensaje){
  return validaIdentify(mensaje);
}

bool ValidaMensaje::validaStatus(const Mensaje& mensaje){
  return mensaje.getEstatus().has_value();
}

bool ValidaMensaje::validaNewStatus(const Mensaje& mensaje){
  return mensaje.getEstatus().has_value() && validaIdentify(mensaje);
}

bool ValidaMensaje::validaUserList(const Mensaje& mensaje){
  return mensaje.getUsers().has_value();
}

bool ValidaMensaje::validaText(const Mensaje& mensaje){
  return mensaje.getText().has_value() && validaIdentify(mensaje);
}

bool ValidaMensaje::validaTextFrom(const Mensaje& mensaje){
  return validaText(mensaje);
}

bool ValidaMensaje::validaPublicText(const Mensaje& mensaje){
  return mensaje.getText().has_value();
}

bool ValidaMensaje::validaPublicTextFrom(const Mensaje& mensaje){
  return mensaje.getText().has_value() && validaIdentify(mensaje);
}

bool ValidaMensaje::validaNewRoom(const Mensaje& mensaje){
  if(!mensaje.getRoomname().has_value())
    return false;

  std::string sala = mensaje.getRoomname().value();

  if(sala.empty() || sala.length() > 16)
    return false;

  return true;
}

bool ValidaMensaje::validaInvite(const Mensaje& mensaje){
  return mensaje.getUsernames().has_value() && validaNewRoom(mensaje);
}

bool ValidaMensaje::validaInvitation(const Mensaje& mensaje){
  return validaIdentify(mensaje) && validaNewRoom(mensaje);
}

bool ValidaMensaje::validaJoinRoom(const Mensaje& mensaje){
  return validaNewRoom(mensaje);
}

bool ValidaMensaje::validaJoinedRoom(const Mensaje& mensaje){
  return validaIdentify(mensaje) && validaNewRoom(mensaje);
}

bool ValidaMensaje::validaRoomUsers(const Mensaje& mensaje){
  return validaNewRoom(mensaje);
}

bool ValidaMensaje::validaRoomUserList(const Mensaje& mensaje){
  return validaNewRoom(mensaje) && mensaje.getUsers().has_value();
}

bool ValidaMensaje::validaRoomText(const Mensaje& mensaje){
  return validaNewRoom(mensaje) && mensaje.getText().has_value();
}

bool ValidaMensaje::validaRoomTextFrom(const Mensaje& mensaje){
  return validaNewRoom(mensaje) && validaText(mensaje);
}

bool ValidaMensaje::validaLeaveRoom(const Mensaje& mensaje){
  return validaNewRoom(mensaje);
}

bool ValidaMensaje::validaLeftRoom(const Mensaje& mensaje){
  return validaNewRoom(mensaje) && validaIdentify(mensaje);
}

bool ValidaMensaje::validaDisconnected(const Mensaje& mensaje){
  return validaIdentify(mensaje);
}

bool ValidaMensaje::valida(const Mensaje& mensaje){
  TipoMensaje tipo = mensaje.getTipo();
  
  switch(tipo){
  case TipoMensaje::IDENTIFY:
    return validaIdentify(mensaje);
    
  case TipoMensaje::RESPONSE:
    return validaResponse(mensaje);
    
  case TipoMensaje::NEW_USER:
    return validaNewUser(mensaje);
    
  case TipoMensaje::STATUS:
    return validaStatus(mensaje);

  case TipoMensaje::NEW_STATUS:
    return validaNewStatus(mensaje);

  case TipoMensaje::USERS:
    return true;

  case TipoMensaje::USER_LIST:
    return validaUserList(mensaje);

  case TipoMensaje::TEXT:
    return validaText(mensaje);

  case TipoMensaje::TEXT_FROM:
    return validaTextFrom(mensaje);

  case TipoMensaje::PUBLIC_TEXT:
    return validaPublicText(mensaje);

  case TipoMensaje::PUBLIC_TEXT_FROM:
    return validaPublicTextFrom(mensaje);

  case TipoMensaje::NEW_ROOM:
    return validaNewRoom(mensaje);

  case TipoMensaje::INVITE:
    return validaInvite(mensaje);

  case TipoMensaje::INVITATION:
    return validaInvitation(mensaje);

  case TipoMensaje::JOIN_ROOM:
    return validaJoinRoom(mensaje);

  case TipoMensaje::JOINED_ROOM:
    return validaJoinedRoom(mensaje);

  case TipoMensaje::ROOM_USERS:
    return validaRoomUsers(mensaje);

  case TipoMensaje::ROOM_USER_LIST:
    return validaRoomUserList(mensaje);

  case TipoMensaje::ROOM_TEXT:
    return validaRoomText(mensaje);

  case TipoMensaje::ROOM_TEXT_FROM:
    return validaRoomTextFrom(mensaje);

  case TipoMensaje::LEAVE_ROOM:
    return validaLeaveRoom(mensaje);

  case TipoMensaje::LEFT_ROOM:
    return validaLeftRoom(mensaje);

  case TipoMensaje::DISCONNECT:
    return true;

  case TipoMensaje::DISCONNECTED:
    return validaDisconnected(mensaje);

  default:
    return false;
  }

  return false;
}
