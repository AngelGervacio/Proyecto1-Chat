/**
 * @file OperacionMensaje.hpp
 * @brief Header de la enumeración OperacionMensaje.
 */

#ifndef OPERACIONMENSAJE_HPP
#define OPERACIONMENSAJE_HPP

/**
 * @brief Enumeración con las operaciones de Mensaje.
 */
enum class OperacionMensaje{
  IDENTIFY,
  TEXT,
  NEW_ROOM,
  INVITE,
  JOIN_ROOM,
  ROOM_USERS,
  ROOM_TEXT,
  LEAVE_ROOM,
  INVALID
};

#endif
