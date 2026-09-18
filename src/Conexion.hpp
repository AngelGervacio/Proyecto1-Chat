/**
 * @file Conexion.hpp
 * @brief Header de la clase Conexion.
 */

#ifndef CONEXION_HPP
#define CONEXION_HPP

#include <memory>
#include <thread>
#include <string>
#include "Usuario.hpp"
#include "Servidor.hpp"

class Servidor;

class Conexion{
private:
  int socketCliente;
  bool activa;
  std::unique_ptr<Usuario> usuario;
  std::thread hilo;
  const std::size_t TAMAÑO_MAXIMO = 1024*1024;
  std::string buffer;
  Servidor& servidor;

public:
  Conexion(int socketCliente, Servidor& servidor);
  ~Conexion();
  void recibeMensajes();
  void enviaMensaje(const Mensaje& mensaje);
  void iniciaHilo();
  bool estaIdentificado();
  const std::unique_ptr<Usuario>& getUsuario() const;
  void setUsuario(std::unique_ptr<Usuario> usuario);
};

#endif
