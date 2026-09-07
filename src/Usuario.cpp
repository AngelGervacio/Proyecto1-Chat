/**
 * @file Usuario.cpp
 * @brief Implementación de la clase Usuario.
 */

#include <stdexcept>
#include "Usuario.hpp"
#include "EstatusUsuario.hpp"

Usuario::Usuario(std::unique_ptr<std::string> nombre){
  
  if(nombre == nullptr || nombre->empty() || nombre->length() > 8)
    throw std::invalid_argument("El nombre de usuario no es valido.");

  this->nombre = std::move(nombre);
  this->estatus = std::make_unique<EstatusUsuario>(EstatusUsuario::ACTIVE);
  
}

std::string Usuario::getNombre(){

  return *nombre;
}

EstatusUsuario Usuario::getEstatus(){

  return *estatus;
}

void Usuario::setEstatus(std::unique_ptr<EstatusUsuario> estatus){

  this->estatus = std::move(estatus);
}
