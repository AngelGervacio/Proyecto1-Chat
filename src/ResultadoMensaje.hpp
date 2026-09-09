/**
 * @file ResultadoMensaje.hpp
 * @brief Header de la enumeración ResultadoMensaje.
 */

#ifndef RESULTADOMENSAJE_HPP
#define RESULTADOMENSAJE_HPP

/**
 * @brief Enumeración con los resultados de Mensaje.
 */
enum class ResultadoMensaje{
  SUCCESS,
  USER_ALREADY_EXISTS,
  NO_SUCH_USER,
  ROOM_ALREADY_EXISTS,
  NO_SUCH_ROOM,
  NOT_INVITED,
  NOT_JOINED,
  NOT_IDENTIFIED,
  INVALID
};

#endif
