/**
 * @file GeneraMensaje.hpp
 * @brief Header de la clase GeneraMensaje.
 */

#ifndef GENERAMENSAJE_HPP
#define GENERAMENSAJE_HPP

#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <list>
#include "TipoMensaje.hpp"
#include "OperacionMensaje.hpp"
#include "ResultadoMensaje.hpp"
#include "EstatusUsuario.hpp"
#include "Mensaje.hpp"

using json = nlohmann::json;

/**
 * @class GeneraMensaje
 * @brief Genera un Mensaje en base a un JSON dado.
 */
class GeneraMensaje{
private:
  // Constructor privado, no permite instanciar la clase.
  GeneraMensaje() = delete;

  /**
   * @brief Genera el tipo de mensaje.
   * @param json El JSON de donde obtener el tipo.
   * @return El tipo de mensaje.
   */
  static TipoMensaje generaTipo(const json& json);

  /**
   * @brief Genera la operación del mensaje.
   * @param json El JSON de donde obtener la operación.
   * @return La operación del mensaje.
   */
  static OperacionMensaje generaOperacion(const json& json);

  /**
   * @brief Genera el resultado del mensaje.
   * @param json El JSON de donde obtener el resultado.
   * @return El resultado del mensaje.
   */
  static ResultadoMensaje generaResultado(const json& json);

  /**
   * @brief Genera el username del mensaje.
   * @param json El JSON de donde obtener el username.
   * @return El username del mensaje.
   */
  static std::string generaUsername(const json& json);

  /**
   * @brief Genera el extra del mensaje.
   * @param json El JSON de donde obtener el extra.
   * @return El extra del mensaje.
   */
  static std::string generaExtra(const json& json);

  /**
   * @brief Genera el estatus del mensaje.
   * @param json El JSON de donde obtener el estatus.
   * @return El estatus del mensaje.
   */
  static EstatusUsuario generaEstatus(const json& json);

  /**
   * @brief Genera los users del mensaje.
   * @param json El JSON de donde obtener los users.
   * @return Los users del mensaje.
   */
  static std::unordered_map<std::string, EstatusUsuario> generaUsers(const json& json);
  
  /**
   * @brief Genera el text del mensaje.
   * @param json El JSON de donde obtener el text.
   * @return El text del mensaje.
   */
  static std::string generaText(const json& json);

  /**
   * @brief Genera el roomname del mensaje.
   * @param json El JSON de donde obtener el roomname.
   * @return El roomname del mensaje.
   */
  static std::string generaRoomname(const json& json);
  
  /**
   * @brief Genera los usernames del mensaje.
   * @param json El JSON de donde obtener los usernames.
   * @return Los usernames del mensaje.
   */
  static std::list<std::string> generaUsernames(const json& json);

public:
  
  /**
   * @brief Genera un Mensaje
   * @param json El JSON de donde obtener el Mensaje.
   * @return El Mensaje generado.
   */
  static Mensaje genera(const json& json);
};

#endif
