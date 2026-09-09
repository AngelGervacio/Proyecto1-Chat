/**
 * @file TipoMensaje.hpp
 * @brief Header de la enumeración TipoMensaje.
 */

#ifndef TIPOMENSAJE_HPP
#define TIPOMENSAJE_HPP

/**
 * @brief Enumeración con los tipos de Mensaje.
 */
enum class TipoMensaje{
  IDENTIFY,
  RESPONSE,
  NEW_USER,
  STATUS,
  NEW_STATUS,
  USERS,
  USER_LIST,
  TEXT,
  TEXT_FROM,
  PUBLIC_TEXT,
  PUBLIC_TEXT_FROM,
  NEW_ROOM,
  INVITE,
  INVITATION,
  JOIN_ROOM,
  JOINED_ROOM,
  ROOM_USERS,
  ROOM_USERS_LIST,
  ROOM_TEXT,
  ROOM_TEXT_FROM,
  LEAVE_ROOM,
  LEFT_ROOM,
  DISCONNECT,
  DISCONNECTED
};

#endif
