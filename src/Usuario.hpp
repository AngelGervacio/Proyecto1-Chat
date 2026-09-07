/**
 * @file Usuario.hpp
 * @brief Header de la clase Usuario.
 */

#ifndef USUARIO_HPP
#define USUARIO_HPP

#include <string>
#include <memory>
#include "EstatusUsuario.hpp"

/**
 * @class Usuario
 * @brief Representa a un Usuario.
 */
class Usuario{

private:

  std::unique_ptr<std::string> nombre; /*!< Nombre del Usuario */
  std::unique_ptr<EstatusUsuario> estatus; /*!< Estatus del Usuario */

public:

  /**
   * @brief Constructor de la clase Usuario.
   * @param nombre El nombre que tendra el Usuario.
   */
  Usuario(std::unique_ptr<std::string> nombre);

  /**
   * @brief Regresa el nombre del Usuario.
   * @return el nombre del Usuario.
   */
  std::string getNombre();

  /**
   * @brief Regresa el estatus del Usuario.
   * @return el estatus del Usuario.
   */
  EstatusUsuario getEstatus();

  /**
   * @brief Actualiza el estatus del Usuario.
   * @param estatus El nuevo estatus del Usuario.
   */
  void setEstatus(std::unique_ptr<EstatusUsuario> estatus);

  
};

#endif
