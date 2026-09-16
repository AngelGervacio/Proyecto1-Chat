/**
 * @file CampoMensaje.hpp
 * @brief Header de la clase CampoMensaje.
 */

#ifndef CAMPOMENSAJE_HPP
#define CAMPOMENSAJE_HPP

#include <string_view>

/**
 * @brief Clase con los campos de Mensaje.
 */
class CampoMensaje{
public:

  static constexpr std::string_view TYPE = "type"; /*!< Tipo de Mensaje */
  static constexpr std::string_view OPERATION = "operation"; /*!< Operacion del Mensaje */
  static constexpr std::string_view RESULT = "result"; /*!< Resultado del Mensaje */ 
  static constexpr std::string_view USERNAME = "username"; /*!< Username del Mensaje */
  static constexpr std::string_view EXTRA = "extra"; /*!< Extra del Mensaje */
  static constexpr std::string_view STATUS = "status"; /*!< Estatus del Mensaje */
  static constexpr std::string_view USERS = "users"; /*!< Users del Mensaje */
  static constexpr std::string_view TEXT = "text"; /*!< Texto del Mensaje */
  static constexpr std::string_view ROOMNAME = "roomname"; /*!< Roomname del Mensaje */
  static constexpr std::string_view USERNAMES = "usernames"; /*!< Usernames del Mensaje */
};

#endif
