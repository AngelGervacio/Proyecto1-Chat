/**
 * @file ResultadoMensaje.hpp
 * @brief Header de la enumeración ResultadoMensaje.
 */

#ifndef RESULTADOMENSAJE_HPP
#define RESULTADOMENSAJE_HPP

#include <nlohmann/json.hpp>

/**
 * @brief Enumeración con los resultados de Mensaje.
 */
enum class ResultadoMensaje{
  NO_VALIDO,
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

NLOHMANN_JSON_SERIALIZE_ENUM(ResultadoMensaje, {
    {ResultadoMensaje::NO_VALIDO, nullptr},
    {ResultadoMensaje::SUCCESS, "SUCCESS"},
    {ResultadoMensaje::USER_ALREADY_EXISTS, "USER_ALREADY_EXISTS"},
    {ResultadoMensaje::NO_SUCH_USER, "NO_SUCH_USER"},
    {ResultadoMensaje::ROOM_ALREADY_EXISTS, "ROOM_ALREADY_EXISTS"},
    {ResultadoMensaje::NO_SUCH_ROOM, "NO_SUCH_ROOM"},
    {ResultadoMensaje::NOT_INVITED, "NOT_INVITED"},
    {ResultadoMensaje::NOT_JOINED, "NOT_JOINED"},
    {ResultadoMensaje::NOT_IDENTIFIED, "NOT_IDENTIFIED"},
    {ResultadoMensaje::INVALID, "INVALID"}
  })

#endif
