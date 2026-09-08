/**
 * @file Usuario.hpp
 * @brief Header de la clase Usuario.
 */

#ifndef USUARIO_HPP
#define USUARIO_HPP

#include <string>
#include "EstatusUsuario.hpp"

/**
 * @class Usuario
 * @brief Representa a un Usuario.
 */
class Usuario{

private:

  std::string nombre; /*!< Nombre del Usuario */
  EstatusUsuario estatus; /*!< Estatus del Usuario */

public:

  /**
   * @brief Constructor de la clase Usuario.
   * @param nombre El nombre que tendra el Usuario.
   */
  Usuario(std::string nombre);

  /**
   * @brief Regresa el nombre del Usuario.
   * @return el nombre del Usuario.
   */
  std::string getNombre() const;

  /**
   * @brief Regresa el estatus del Usuario.
   * @return el estatus del Usuario.
   */
  EstatusUsuario getEstatus() const;

  /**
   * @brief Actualiza el estatus del Usuario.
   * @param estatus El nuevo estatus del Usuario.
   */
  void setEstatus(EstatusUsuario estatus);

  
};

#endif
