/**
 * @file Cliente.hpp
 * @brief Header de la clase Cliente.
 */

#ifndef CLIENTE_HPP
#define CLIENTE_HPP

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include <list>
#include <functional>
#include "Usuario.hpp"
#include "Conexion.hpp"
#include "TipoMensaje.hpp"
#include "EstatusUsuario.hpp"
#include "Mensaje.hpp"

class Cliente{
private:
  std::unique_ptr<Conexion> conexion; /*!< Conexión del cliente */
  Usuario usuario; /*!< Usuario del cliente */
  std::list<std::function<void(const Mensaje&)>> escuchas; /*!< Lista de escuchas */

  /**
   * @brief Procesa el mensaje recibido.
   * @param mensaje El mensaje recibido.
   */
  void mensajeRecibido(const Mensaje& mensaje);

public:
  
  /**
   * @brief Constructor del cliente.
   * @param ipServidor La ip al servidor donde se conectara.
   * @param puerto El puerto del servidor.
   */
  Cliente(std::string ipServidor, int puerto);

  /**
   * @brief Identifica al cliente.
   * @param nombre El nombre de usuario.
   */
  void identificarse(std::string nombre);

  /**
   * @brief Cambia el estatus del usuario.
   * @param estatus El nuevo estatus.
   */
  void cambiarEstatus(EstatusUsuario estatus);

  /**
   * @brief Obtiene la lista de usuarios conectados.
   */
  void obtenerUsuarios();

  /**
   * @brief Envia un texto privado.
   * @param usuario El usuario al que se enviara el mensaje.
   * @param texto El texto que se enviara.
   */
  void enviarTexto(std::string usuario, std::string texto);

  /**
   * @brief Envia un texto publico.
   * @param texto El texto que se enviara.
   */
  void enviarTextoPublico(std::string texto);

  /**
   * @brief Crea una sala.
   * @param sala El nombre de la sala.
   */
  void crearSala(std::string sala);

  /**
   * @brief Invita usuarios a una sala.
   * @param usuarios Los usuarios a invitar.
   * @param sala La sala a la cual se invitara.
   */
  void invitar(std::list<std::string> usuarios, std::string sala);

  /**
   * @brief Entrar a una sala.
   * @param sala La sala a la que se entrara.
   */
  void entrarSala(std::string sala);

  /**
   * @brief Obtiene la lista de usuarios conectados a una sala.
   * @param sala La sala de donde obtener la lista.
   */
  void obtenerUsuariosSala(std::string sala);

  /**
   * @brief Envia texto a una sala.
   * @param sala La sala a donde se enviara el texto.
   * @param texto El texto a enviar.
   */
  void enviarTextoSala(std::string sala, std::string texto);

  /**
   * @brief Salir de una sala.
   * @param sala La sala de donde se saldra.
   */
  void salirSala(std::string sala);

  /**
   * @brief Desconecta el cliente del servidor.
   */
  void desconectar();

  /**
   * @brief Agrega un escucha a la lista de escuchas.
   * @param escucha El escucha a agregar.
   */
  void agregaEscucha(std::function<void(const Mensaje&)> escucha);
};

#endif
