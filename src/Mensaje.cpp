/**
 * @file Mensaje.cpp
 * @brief Implementación de la clase Mensaje.
 */

#include <string>
#include <optional>
#include <list>
#include "Mensaje.hpp"
#include "Usuario.hpp"
#include "EstatusUsuario.hpp"
#include "TipoMensaje.hpp"
#include "OperacionMensaje.hpp"
#include "ResultadoMensaje.hpp"
  
Mensaje::Builder& Mensaje::Builder::setTipo(TipoMensaje tipo){
  this->tipo = tipo;
  return *this;
}
  
Mensaje::Builder& Mensaje::Builder::setOperacion(OperacionMensaje operacion){
  this->operacion = operacion;
  return *this;
}
  
Mensaje::Builder& Mensaje::Builder::setResultado(ResultadoMensaje resultado){
  this->resultado = resultado;
  return *this;
}
  
Mensaje::Builder& Mensaje::Builder::setUsername(std::string username){
  this->username = username;
  return *this;
}
  
Mensaje::Builder& Mensaje::Builder::setExtra(std::string extra){
  this->extra = extra;
  return *this;
}
  
Mensaje::Builder& Mensaje::Builder::setEstatus(EstatusUsuario estatus){
  this->estatus = estatus;
  return *this;
}
  
Mensaje::Builder& Mensaje::Builder::setUsers(std::list<Usuario> users){
  this->users = users;
  return *this;
}
  
Mensaje::Builder& Mensaje::Builder::setText(std::string text){
  this->text = text;
  return *this;
}
    
Mensaje::Builder& Mensaje::Builder::setRoomname(std::string roomname){
  this->roomname = roomname;
  return *this;
}
    
Mensaje::Builder& Mensaje::Builder::setUsernames(std::list<std::string> usernames){
  this->usernames = usernames;
  return *this;
}
  
Mensaje Mensaje::Builder::build(){
  return Mensaje(tipo.value(),
		 operacion,
		 resultado,
		 username,
		 extra,
		 estatus,
		 users,
		 text,
		 roomname,
		 usernames);
}

Mensaje::Mensaje(TipoMensaje tipo,
		 std::optional<OperacionMensaje> operacion,
		 std::optional<ResultadoMensaje> resultado,
		 std::optional<std::string> username,
		 std::optional<std::string> extra,
		 std::optional<EstatusUsuario> estatus,
		 std::optional<std::list<Usuario>> users,
		 std::optional<std::string> text,
		 std::optional<std::string> roomname,
		 std::optional<std::list<std::string>> usernames){
  this->tipo = tipo;
  this->operacion = operacion;
  this->resultado = resultado;
  this->username = username;
  this->extra = extra;
  this->estatus = estatus;
  this->users = users;
  this->text = text;
  this->roomname = roomname;
  this->usernames = usernames;
}

TipoMensaje Mensaje::getTipo() const {
  return tipo;
}

std::optional<OperacionMensaje> Mensaje::getOperacion() const {
  return operacion;
}

std::optional<ResultadoMensaje> Mensaje::getResultado() const {
  return resultado;
}

std::optional<std::string> Mensaje::getUsername() const {
  return username;
}

std::optional<std::string> Mensaje::getExtra() const {
  return extra;
}

std::optional<EstatusUsuario> Mensaje::getEstatus() const {
  return estatus;
}

std::optional<std::list<Usuario>> Mensaje::getUsers() const {
  return users;
}

std::optional<std::string> Mensaje::getText() const {
  return text;
}

std::optional<std::string> Mensaje::getRoomname() const {
  return roomname;
}

std::optional<std::list<std::string>> Mensaje::getUsernames() const {
  return usernames;
}
