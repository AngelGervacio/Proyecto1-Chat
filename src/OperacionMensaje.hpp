/**
 * @file OperacionMensaje.hpp
 * @brief Header de la enumeración OperacionMensaje.
 */

#ifndef OPERACIONMENSAJE_HPP
#define OPERACIONMENSAJE_HPP

#include <nlohmann/json.hpp>

/**
 * @brief Enumeración con las operaciones de Mensaje.
 */
enum class OperacionMensaje{
  NO_VALIDO,
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

NLOHMANN_JSON_SERIALIZE_ENUM(OperacionMensaje, {
    {OperacionMensaje::NO_VALIDO, nullptr},
    {OperacionMensaje::IDENTIFY, "IDENTIFY"},
    {OperacionMensaje::TEXT, "TEXT"},
    {OperacionMensaje::NEW_ROOM, "NEW_ROOM"},
    {OperacionMensaje::INVITE, "INVITE"},
    {OperacionMensaje::JOIN_ROOM, "JOIN_ROOM"},
    {OperacionMensaje::ROOM_USERS, "ROOM_USERS"},
    {OperacionMensaje::ROOM_TEXT, "ROOM_TEXT"},
    {OperacionMensaje::LEAVE_ROOM, "LEAVE_ROOM"},
    {OperacionMensaje::INVALID, "INVALID"}
  })

#endif
