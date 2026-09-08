/**
 * @file Usuario.cpp
 * @brief Implementación de la clase Usuario.
 */

#include <stdexcept>
#include "Usuario.hpp"
#include "EstatusUsuario.hpp"

Usuario::Usuario(std::string nombre){
  if(nombre.empty() || nombre.length() > 8)
    throw std::invalid_argument("El nombre de usuario no es valido.");

  this->nombre = nombre;
  this->estatus = EstatusUsuario::ACTIVE;
  
}

std::string Usuario::getNombre() const {
  return nombre;
}

EstatusUsuario Usuario::getEstatus() const {
  return estatus;
}

void Usuario::setEstatus(EstatusUsuario estatus){
  this->estatus = estatus;
}
