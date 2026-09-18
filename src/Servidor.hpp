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
#include <mutex>
#include <iostream>
#include "EstatusUsuario.hpp"
#include "Conexion.hpp"

using json = nlohmann::json;

class Conexion;
class Mensaje;

class Servidor{
private:
  std::list<std::unique_ptr<Conexion>> conexiones;
  std::unordered_map<std::string, EstatusUsuario> usuarios;
  std::mutex mutex;
  int puerto;
  int socketServidor;
  bool enEjecucion;

  void aceptaConexion();
  void detenerServidor();

public:
  
  Servidor(int puerto);
  void sirve();
  void mensajeRecibido(Conexion& conexion, const Mensaje& mensaje);
  
};

#endif
