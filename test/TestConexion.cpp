/**
 * @file TestConexion.cpp
 * @brief Pruebas unitarias de la clase Conexion.
 */

#include <gtest/gtest.h>
#include <list>
#include <functional>
#include <sys/socket.h>
#include <netinet/in.h>
#include <chrono>
#include <thread>
#include <nlohmann/json.hpp>
#include "Mensaje.hpp"
#include "TipoMensaje.hpp"
#include "GeneraJSON.hpp"
#include "Conexion.hpp"

using json = nlohmann::json;

/**
 * @brief Prueba el constructor y los metodos estaActiva()
 * y getId() de la clase Conexion. Comprueba que se construya
 * correctamente la Conexion.
 */
TEST(ConexionTest, ConstructorConexion){
  Conexion conexion1(socket(AF_INET, SOCK_STREAM, 0));
  Conexion conexion2(socket(AF_INET, SOCK_STREAM, 0));

  EXPECT_TRUE(conexion1.estaActiva());
  EXPECT_TRUE(conexion2.estaActiva());
  EXPECT_NE(conexion1.getId(), conexion2.getId());

  int idAnterior = conexion2.getId();
  for(int i = 0; i < 50; i++){
    Conexion cnx(socket(AF_INET, SOCK_STREAM, 0));
    EXPECT_TRUE(cnx.estaActiva());
    EXPECT_EQ(cnx.getId(), ++idAnterior);
  }
}

/**
 * @brief Prueba los metodos iniciaHilo() y recibeMensajes() de la clase Conexion.
 * Comprueba que la Conexion reciba mensajes correctamente.
 */
TEST(ConexionTest, RecibirMensaje){
  int sockets[2];
  socketpair(AF_UNIX, SOCK_STREAM, 0, sockets);
  
  Conexion conexionReceptora(sockets[0]);
  Conexion conexionMensajero(sockets[1]);
  
  Mensaje mensajeRecibido = Mensaje::Builder().build();
  conexionReceptora.agregaEscucha([&](Conexion&, const Mensaje& mensaje) {
    mensajeRecibido = mensaje;
  });

  conexionReceptora.iniciaHilo();

  Mensaje mensajeEnviado = Mensaje::Builder()
    .setTipo(TipoMensaje::IDENTIFY)
    .setUsername("Alice")
    .build();
  
  conexionMensajero.enviaMensaje(mensajeEnviado); 
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  EXPECT_EQ(mensajeRecibido.getTipo(), mensajeEnviado.getTipo());
  EXPECT_EQ(mensajeRecibido.getUsername().value(), mensajeEnviado.getUsername().value());
}

/**
 * @brief Prueba el metodo enviaMensaje() de la clase Conexion.
 * Comprueba que la Conexion envie mensajes correctamente.
 */
TEST(ConexionTest, EnviarMensaje){
  int sockets[2];
  socketpair(AF_UNIX, SOCK_STREAM, 0, sockets);
  
  Conexion conexionMensajero(sockets[0]);
  
  Mensaje mensajeEnviado = Mensaje::Builder()
    .setTipo(TipoMensaje::IDENTIFY)
    .setUsername("Alice")
    .build();
  
  conexionMensajero.enviaMensaje(mensajeEnviado); 

  json jsonEnviado = GeneraJSON::genera(mensajeEnviado);

  char datos[1024];
  auto bytesLeidos = read(sockets[1], datos, sizeof(datos));
  std::string linea(datos, bytesLeidos);
  
  json jsonRecibido = json::parse(linea);
  
  EXPECT_EQ(jsonRecibido, jsonEnviado);
}

/**
 * @brief Prueba el metodo desconecta() de la clase Conexion.
 * Comprueba que la Conexion se desconecte correctamente.
 */
TEST(ConexionTest, DesconectaConexion){
  int sockets[2];
  socketpair(AF_UNIX, SOCK_STREAM, 0, sockets);
  
  Conexion conexion(sockets[0]);
  
  EXPECT_TRUE(conexion.estaActiva());

  conexion.desconecta();

  EXPECT_FALSE(conexion.estaActiva());

  char datos[1024];
  EXPECT_EQ(read(sockets[1], datos, sizeof(datos)), 0);
}
