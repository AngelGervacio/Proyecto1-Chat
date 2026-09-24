/**
 * @file Servidor.hpp
 * @brief Header de la clase Servidor.
 */

#ifndef SERVIDOR_HPP
#define SERVIDOR_HPP

#include <nlohmann/json.hpp>
#include <list>
#include <sys/socket.h>
#include <netinet/in.h>
#include <memory>
#include <cstdlib>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include <iostream>
#include "Conexion.hpp"
#include "Mensaje.hpp"
#include "Usuario.hpp"
#include "TipoMensaje.hpp"
#include "OperacionMensaje.hpp"
#include "ResultadoMensaje.hpp"

using json = nlohmann::json;

class Conexion;
class Mensaje;

/**
 * @class Servidor
 * @brief Servidor del chat.
 */
class Servidor{
private:

  /**
   * @struct ConexionUsuario
   * @brief Estructura para identificar conexiones.
   */
  struct ConexionUsuario{
    std::unique_ptr<Conexion> conexion; /*!< Conexion */
    std::string username; /*!< Nombre de usuario de la conexión */
  };

  /**
   * @struct Sala
   * @brief Representación de una sala.
   */
  struct Sala{
    std::string nombre; /*!< Nombre de la sala */
    std::unordered_map<std::string, EstatusUsuario> usuariosEnSala; /*!< Diccionario de usuarios en la sala */
    std::unordered_map<std::string, Conexion*> miembros; /*!< Diccionario de miembros conectados */
    std::unordered_set<std::string> invitados; /*!< Conjunto de usuarios invitados */
  };
  
  std::list<ConexionUsuario> conexiones; /*!< Lista de conexiones. */
  std::unordered_set<int> idsConectadas; /*!< Conjunto de ids de conexiones */
  std::unordered_map<std::string, EstatusUsuario> usuariosIdentificados; /*!< Diccionario de usuarios */
  std::unordered_map<std::string, Sala> salas; /*!< Diccionario de salas */
  std::mutex mutex; /*!< Mutex del servidor */
  int puerto; /*!< Puerto del servidor */
  int socketServidor; /*!< Socket del servidor */
  bool enEjecucion; /*!< Booleano para servidor en ejecución */

  /**
   * @brief Acepta conexiones y lo agrega a la lista del servidor.
   */
  void aceptaConexion();

  /**
   * @brief Identifica a la conexion.
   * @param conexion La conexión a identificar.
   * @param mensaje El mensaje enviado.
   */
  void identifica(Conexion& conexion, const Mensaje& mensaje);

  /**
   * @brief Cambia el estatus.
   * @param conexion La conexion que cambio su estatus.
   * @param mensaje El mensaje enviado.
   */
  void estatus(Conexion& conexion, const Mensaje& mensaje);
  
  /**
   * @brief Envia un texto privado.
   * @param conexion La conexion que envio el texto.
   * @param mensaje El mensaje enviado.
   */
  void texto(Conexion& conexion, const Mensaje& mensaje);
  
  /**
   * @brief Envia un texto publico.
   * @param conexion La conexion que envio el texto.
   * @param mensaje El mensaje enviado.
   */
  void textoPublico(Conexion& conexion, const Mensaje& mensaje);
  
  /**
   * @brief Crea una sala.
   * @param conexion La conexion que creó la sala.
   * @param mensaje El mensaje enviado.
   */
  void nuevaSala(Conexion& conexion, const Mensaje& mensaje);
  
  /**
   * @brief Invita usuarios a una sala.
   * @param conexion La conexion que envio las invitaciones.
   * @param mensaje El mensaje enviado.
   */
  void invita(Conexion& conexion, const Mensaje& mensaje);
  
  /**
   * @brief Entra a una sala.
   * @param conexion La conexion que entra a la sala.
   * @param mensaje El mensaje enviado.
   */
  void entrarSala(Conexion& conexion, const Mensaje& mensaje);
  
  /**
   * @brief Regresa la lista de usuarios conectados.
   * @param conexion La conexion que solicito la lista.
   * @param mensaje El mensaje enviado.
   */
  void usuariosSala(Conexion& conexion, const Mensaje& mensaje);
  
  /**
   * @brief Envia un texto a una sala.
   * @param conexion La conexion que envio el texto.
   * @param mensaje El mensaje enviado.
   */
  void textoSala(Conexion& conexion, const Mensaje& mensaje);
  
  /**
   * @brief Sale de una sala.
   * @param conexion La conexion que sale de la sala.
   * @param mensaje El mensaje enviado.
   */
  void salirSala(Conexion& conexion, const Mensaje& mensaje);
  
  /**
   * @brief Desconecta una conexion del servidor.
   * @param conexion La conexion que se va a desconectar.
   */
  void desconecta(Conexion& conexion);

public:

  /**
   * @brief Construye el servidor.
   * @param puerto El puerto donde estara el servidor.
   */
  Servidor(int puerto);

  /**
   * @brief Destructor del servidor.
   */
  ~Servidor();

  /**
   * @brief Pone al servidor en ejecución.
   */
  void sirve();

  /**
   * @brief Procesa los mensajes enviados por las conexiones.
   * @param conexion La conexion que envio el mensaje.
   * @param mensaje El mensaje enviado.
   */
  void mensajeRecibido(Conexion& conexion, const Mensaje& mensaje);

  /**
   * Detiene la ejecución del servidor.
   */
  void detenerServidor();
};

#endif
