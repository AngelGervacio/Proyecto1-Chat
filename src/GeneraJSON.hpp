/**
 * @file GeneraJSON.hpp
 * @brief Header de la clase GeneraJSON.
 */

#ifndef GENERAJSON_HPP
#define GENERAJSON_HPP

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
 * @class GeneraJSON
 * @brief Genera un JSON en base a un Mensaje dado.
 */
class GeneraJSON{
private:
  // Constructor privado, no permite instanciar la clase.
  GeneraJSON() = delete;

  /** 
   * @brief Genera el tipo de mensaje en JSON.
   * @param tipo El tipo de mensaje.
   * @return Un JSON con tipo de mensaje.
   */
  static json generaTipo(const TipoMensaje& tipo);

  /** 
   * @brief Genera la operación del mensaje en JSON.
   * @param operacion La operación del mensaje.
   * @return Un JSON con la operación del mensaje.
   */
  static json generaOperacion(const OperacionMensaje& operacion);

  /** 
   * @brief Genera el resultado del mensaje en JSON.
   * @param resultado El resultado del mensaje.
   * @return Un JSON con el resultado del mensaje.
   */
  static json generaResultado(const ResultadoMensaje& resultado);

  /** 
   * @brief Genera el estatus del mensaje en JSON.
   * @param estatus El estatus del mensaje.
   * @return Un JSON con el estatus del mensaje.
   */
  static json generaEstatus(const EstatusUsuario& estatus);

  /** 
   * @brief Genera los users del mensaje en JSON.
   * @param users Los users del mensaje.
   * @return Un JSON con los users del mensaje.
   */
  static json generaUsers(const std::unordered_map<std::string, EstatusUsuario>& users);

  /** 
   * @brief Genera los usernames del mensaje en JSON.
   * @param usernames Los usernames del mensaje.
   * @return Un JSON con los usernames del mensaje.
   */
  static json generaUsernames(const std::list<std::string>& usernames);

  /** 
   * @brief Obtiene el username del mensaje.
   * @param mensaje El mensaje de donde se obtendra el username.
   * @return El username del mensaje.
   */
  static std::string obtieneUsername(const Mensaje& mensaje);

  /** 
   * @brief Obtiene el username del mensaje.
   * @param mensaje El mensaje de donde se obtendra el username.
   * @return El username del mensaje.
   */
  static std::string obtieneRoomname(const Mensaje& mensaje);

  /** 
   * @brief Obtiene el username del mensaje.
   * @param mensaje El mensaje de donde se obtendra el username.
   * @return El username del mensaje.
   */
  static std::string obtieneText(const Mensaje& mensaje);

public:

  /** 
   * @brief Genera un JSON en base a un mensaje.
   * @param mensaje El mensaje que se generara.
   * @return Un JSON generado en base al mensaje.
   */
  static json genera(const Mensaje& mensaje);
  
};

#endif
