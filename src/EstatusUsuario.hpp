/**
 * @file EstatusUsuario.hpp
 * @brief Header de la clase EstatusUsuario.
 */

#ifndef ESTATUSUSUARIO_HPP
#define ESTATUSUSUARIO_HPP

#include <nlohmann/json.hpp>

/**
 * @brief Enumeración con los posibles estatus del Usuario.
 */
enum class EstatusUsuario{
  NO_VALIDO,
  ACTIVE,
  AWAY,
  BUSY
};

NLOHMANN_JSON_SERIALIZE_ENUM(EstatusUsuario, {
    {EstatusUsuario::NO_VALIDO, nullptr},
    {EstatusUsuario::ACTIVE, "ACTIVE"},
    {EstatusUsuario::AWAY, "AWAY"},
    {EstatusUsuario::BUSY, "BUSY"}
  })

#endif
