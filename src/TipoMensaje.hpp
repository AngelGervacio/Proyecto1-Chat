/**
 * @file TipoMensaje.hpp
 * @brief Header de la enumeración TipoMensaje.
 */

#ifndef TIPOMENSAJE_HPP
#define TIPOMENSAJE_HPP

#include <nlohmann/json.hpp>

/**
 * @brief Enumeración con los tipos de Mensaje.
 */
enum class TipoMensaje{
  NO_VALIDO,
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
  ROOM_USER_LIST,
  ROOM_TEXT,
  ROOM_TEXT_FROM,
  LEAVE_ROOM,
  LEFT_ROOM,
  DISCONNECT,
  DISCONNECTED
};

NLOHMANN_JSON_SERIALIZE_ENUM(TipoMensaje, {
    {TipoMensaje::NO_VALIDO, nullptr},
    {TipoMensaje::IDENTIFY, "IDENTIFY"},
    {TipoMensaje::RESPONSE, "RESPONSE"},
    {TipoMensaje::NEW_USER, "NEW_USER"},
    {TipoMensaje::STATUS, "STATUS"},
    {TipoMensaje::NEW_STATUS, "NEW_STATUS"},
    {TipoMensaje::USERS, "USERS"},
    {TipoMensaje::USER_LIST, "USER_LIST"},
    {TipoMensaje::TEXT, "TEXT"},
    {TipoMensaje::TEXT_FROM, "TEXT_FROM"},
    {TipoMensaje::PUBLIC_TEXT, "PUBLIC_TEXT"},
    {TipoMensaje::PUBLIC_TEXT_FROM, "PUBLIC_TEXT_FROM"},
    {TipoMensaje::NEW_ROOM, "NEW_ROOM"},
    {TipoMensaje::INVITE, "INVITE"},
    {TipoMensaje::INVITATION, "INVITATION"},
    {TipoMensaje::JOIN_ROOM, "JOIN_ROOM"},
    {TipoMensaje::JOINED_ROOM, "JOINED_ROOM"},
    {TipoMensaje::ROOM_USERS, "ROOM_USERS"},
    {TipoMensaje::ROOM_USER_LIST, "ROOM_USER_LIST"},
    {TipoMensaje::ROOM_TEXT, "ROOM_TEXT"},
    {TipoMensaje::ROOM_TEXT_FROM, "ROOM_TEXT_FROM"},
    {TipoMensaje::LEAVE_ROOM, "LEAVE_ROOM"},
    {TipoMensaje::LEFT_ROOM, "LEFT_ROOM"},
    {TipoMensaje::DISCONNECT, "DISCONNECT"},
    {TipoMensaje::DISCONNECTED, "DISCONNECTED"}
  })

#endif
