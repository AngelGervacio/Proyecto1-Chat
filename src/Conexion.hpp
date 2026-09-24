/**
 * @file Conexion.hpp
 * @brief Header de la clase Conexion.
 */

#ifndef CONEXION_HPP
#define CONEXION_HPP

#include <thread>
#include <string>
#include <list>
#include <stdexcept>
#include <functional>
#include <sys/socket.h>

class Mensaje;

/**
 * @class Conexion
 * @brief Representa una Conexión.
 */
class Conexion{
private:
  static int contadorId; /*!< Contador de conexiones */
  static constexpr std::size_t TAMAÑO_MAXIMO = 1024*1024; /*!< Tamaño maximo de los mensajes */
  int socketCliente; /*!< Socket de la Conexión */
  bool activa; /*!< Estado de la Conexión */
  int id; /*!< Id de la Conexión */
  bool identificado; /*!< Esta identificado */
  std::thread hilo; /*!< Hilo de ejecución de la Conexión */
  std::string buffer; /*!< Almacena texto recibido */
  std::list<std::function<void(Conexion&, const Mensaje&)>> escuchas; /*!< Lista de escuchas */

  /**
   * @brief Envia un mensaje de respuesta invalido.
   */
  void respuestaInvalida();
  
public:
  /**
   * @brief Constructor de la clase Conexion.
   * @param socketClient El socket de la Conexión
   */
  Conexion(int socketCliente);
  
  /**
   * @brief Destructor de la clase Conexion.
   */
  ~Conexion();

  /**
   * @brief Inica el hilo de ejecución de la Conexion.
   */
  void iniciaHilo();

  /**
   * @brief Recibe mensajes enviados.
   */
  void recibeMensajes();

  /**
   * @brief Envia un mensaje al Servidor.
   * @param mensaje El mensaje a enviar.
   */
  void enviaMensaje(const Mensaje& mensaje);

  /**
   * @brief Agrega un escucha a la lista de escuchas.
   * @param escucha El escucha a agregar.
   */
  void agregaEscucha(std::function<void(Conexion&, const Mensaje&)> escucha);

  /**
   * @brief Desconecta a la Conexion.
   */
  void desconecta();

  /**
   * @brief Regresa el estado de la Conexion.
   * @return El estado de la Conexion.
   */
  bool estaActiva();

  /**
   * @brief Regresa el id de la Conexion.
   * @return El id de la Conexion.
   */
  int getId();

  /**
   * @brief Establece en verdadero si la conexión se identifico.
   */
  void seIdentifico();

  /**
   * @brief Regresa identificado.
   * @return Si ya se identifico la conexión.
   */
  bool getIdentificado();
};

#endif
