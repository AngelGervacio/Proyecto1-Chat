/**
 * @file TestGeneraMensaje.cpp
 * @brief Pruebas unitarias de la clase GeneraMensaje.
 */

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <list>
#include "TipoMensaje.hpp"
#include "OperacionMensaje.hpp"
#include "ResultadoMensaje.hpp"
#include "EstatusUsuario.hpp"
#include "Mensaje.hpp"
#include "CampoMensaje.hpp"
#include "GeneraMensaje.hpp"

using json = nlohmann::json;

/**
 * @brief Prueba el metodo generaTipo() de la clase GeneraMensaje.
 * Comprueba que se obtenga el tipo del JSON correctamente.
 */
TEST(GeneraMensajeTest, GeneraTipo){
  json json = {
    {CampoMensaje::TYPE, TipoMensaje::IDENTIFY},
    {CampoMensaje::USERNAME, "Alice"}
  };

  Mensaje mensaje = GeneraMensaje::genera(json);

  EXPECT_EQ(mensaje.getTipo(), TipoMensaje::IDENTIFY);

  json = {
    {CampoMensaje::TYPE, TipoMensaje::DISCONNECT}
  };

  mensaje = GeneraMensaje::genera(json);
  
  EXPECT_EQ(mensaje.getTipo(), TipoMensaje::DISCONNECT);

  json = {
    {CampoMensaje::TYPE, "NO_EXISTE"}
  };

  EXPECT_THROW(GeneraMensaje::genera(json), std::invalid_argument);
}

/**
 * @brief Prueba el metodo generaOperacion() de la clase GeneraMensaje.
 * Comprueba que se obtenga la operación del JSON correctamente.
 */
TEST(GeneraMensajeTest, GeneraOperacion){
  json json = {
    {CampoMensaje::TYPE, TipoMensaje::RESPONSE},
    {CampoMensaje::OPERATION, OperacionMensaje::IDENTIFY},
    {CampoMensaje::RESULT, ResultadoMensaje::SUCCESS},
    {CampoMensaje::EXTRA, "Alice"}
  };
  
  Mensaje mensaje = GeneraMensaje::genera(json);

  EXPECT_TRUE(mensaje.getOperacion().has_value());
  EXPECT_EQ(mensaje.getOperacion().value(), OperacionMensaje::IDENTIFY);
  
  json = {
    {CampoMensaje::TYPE, TipoMensaje::RESPONSE},
    {CampoMensaje::RESULT, ResultadoMensaje::SUCCESS},
    {CampoMensaje::EXTRA, "Alice"}
  };
  
  EXPECT_THROW(GeneraMensaje::genera(json), std::invalid_argument);

  json = {
    {CampoMensaje::TYPE, TipoMensaje::RESPONSE},
    {CampoMensaje::OPERATION, "NO_EXISTE"},
    {CampoMensaje::RESULT, ResultadoMensaje::SUCCESS},
    {CampoMensaje::EXTRA, "Alice"}
  };
  
  EXPECT_THROW(GeneraMensaje::genera(json), std::invalid_argument);
}

/**
 * @brief Prueba el metodo generaResultado() de la clase GeneraMensaje.
 * Comprueba que se obtenga el resultado del JSON correctamente.
 */
TEST(GeneraMensajeTest, GeneraResultado){
  json json = {
    {CampoMensaje::TYPE, TipoMensaje::RESPONSE},
    {CampoMensaje::OPERATION, OperacionMensaje::IDENTIFY},
    {CampoMensaje::RESULT, ResultadoMensaje::SUCCESS},
    {CampoMensaje::EXTRA, "Alice"}
  };
  
  Mensaje mensaje = GeneraMensaje::genera(json);

  EXPECT_TRUE(mensaje.getResultado().has_value());
  EXPECT_EQ(mensaje.getResultado().value(), ResultadoMensaje::SUCCESS);
  
  json = {
    {CampoMensaje::TYPE, TipoMensaje::RESPONSE},
    {CampoMensaje::OPERATION, OperacionMensaje::IDENTIFY},
    {CampoMensaje::EXTRA, "Alice"}
  };
  
  EXPECT_THROW(GeneraMensaje::genera(json), std::invalid_argument);

  json = {
    {CampoMensaje::TYPE, TipoMensaje::RESPONSE},
    {CampoMensaje::OPERATION, OperacionMensaje::IDENTIFY},
    {CampoMensaje::RESULT, "NO_EXISTE"},
    {CampoMensaje::EXTRA, "Alice"}
  };
  
  EXPECT_THROW(GeneraMensaje::genera(json), std::invalid_argument);
}

/**
 * @brief Prueba el metodo generaUsername() de la clase GeneraMensaje.
 * Comprueba que se obtenga el username del JSON correctamente.
 */
TEST(GeneraMensajeTest, GeneraUsername){
  json json = {
    {CampoMensaje::TYPE, TipoMensaje::IDENTIFY},
    {CampoMensaje::USERNAME, "Alice"}
  };
  
  Mensaje mensaje = GeneraMensaje::genera(json);

  EXPECT_TRUE(mensaje.getUsername().has_value());
  EXPECT_EQ(mensaje.getUsername().value(), "Alice");

  json = {
    {CampoMensaje::TYPE, TipoMensaje::IDENTIFY}
  };
  
  EXPECT_THROW(GeneraMensaje::genera(json), std::invalid_argument);
}

/**
 * @brief Prueba el metodo generaExtra() de la clase GeneraMensaje.
 * Comprueba que se obtenga el extra del JSON correctamente.
 */
TEST(GeneraMensajeTest, GeneraExtra){
  json json = {
    {CampoMensaje::TYPE, TipoMensaje::RESPONSE},
    {CampoMensaje::OPERATION, OperacionMensaje::IDENTIFY},
    {CampoMensaje::RESULT, ResultadoMensaje::SUCCESS},
    {CampoMensaje::EXTRA, "Alice"}
  };
  
  Mensaje mensaje = GeneraMensaje::genera(json);

  EXPECT_TRUE(mensaje.getExtra().has_value());
  EXPECT_EQ(mensaje.getExtra().value(), "Alice");

  json = {
    {CampoMensaje::TYPE, TipoMensaje::RESPONSE},
    {CampoMensaje::OPERATION, OperacionMensaje::IDENTIFY},
    {CampoMensaje::RESULT, ResultadoMensaje::SUCCESS},
  };
  
  EXPECT_THROW(GeneraMensaje::genera(json), std::invalid_argument);
}

/**
 * @brief Prueba el metodo generaEstatus() de la clase GeneraMensaje.
 * Comprueba que se obtenga el estatus del JSON correctamente.
 */
TEST(GeneraMensajeTest, GeneraEstatus){
  json json = {
    {CampoMensaje::TYPE, TipoMensaje::STATUS},
    {CampoMensaje::STATUS, EstatusUsuario::ACTIVE}
  };
  
  Mensaje mensaje = GeneraMensaje::genera(json);

  EXPECT_TRUE(mensaje.getEstatus().has_value());
  EXPECT_EQ(mensaje.getEstatus().value(), EstatusUsuario::ACTIVE);

  json = {
    {CampoMensaje::TYPE, TipoMensaje::STATUS},
    {CampoMensaje::STATUS, "NO_EXISTE"}
  };
  
  EXPECT_THROW(GeneraMensaje::genera(json), std::invalid_argument);

  json = {
    {CampoMensaje::TYPE, TipoMensaje::STATUS}
  };
  
  EXPECT_THROW(GeneraMensaje::genera(json), std::invalid_argument);
}

/**
 * @brief Prueba el metodo generaUsers() de la clase GeneraMensaje.
 * Comprueba que se obtenga los users del JSON correctamente.
 */
TEST(GeneraMensajeTest, GeneraUsers){
  std::unordered_map<std::string, EstatusUsuario> users = {
    {"Alice", EstatusUsuario::ACTIVE},
    {"Bob", EstatusUsuario::AWAY},
    {"Charlie", EstatusUsuario::BUSY}
  };
  
  json json = {
    {CampoMensaje::TYPE, TipoMensaje::USER_LIST},
    {CampoMensaje::USERS, users}
  };
  
  Mensaje mensaje = GeneraMensaje::genera(json);

  EXPECT_TRUE(mensaje.getUsers().has_value());
  EXPECT_EQ(mensaje.getUsers().value(), users);

  json = {
    {CampoMensaje::TYPE, TipoMensaje::USER_LIST}
  };
  
  EXPECT_THROW(GeneraMensaje::genera(json), std::invalid_argument);

  json = {
    {CampoMensaje::TYPE, TipoMensaje::USER_LIST},
    {CampoMensaje::USERS, {{"Alice", "NO_EXISTE"}}}
  };
  
  EXPECT_THROW(GeneraMensaje::genera(json), std::invalid_argument);
}

/**
 * @brief Prueba el metodo generaText() de la clase GeneraMensaje.
 * Comprueba que se obtenga el text del JSON correctamente.
 */
TEST(GeneraMensajeTest, GeneraText){
  json json = {
    {CampoMensaje::TYPE, TipoMensaje::TEXT},
    {CampoMensaje::USERNAME, "Alice"},
    {CampoMensaje::TEXT, "Texto necesario"}
  };
  
  Mensaje mensaje = GeneraMensaje::genera(json);

  EXPECT_TRUE(mensaje.getText().has_value());
  EXPECT_EQ(mensaje.getText().value(), "Texto necesario");

  json = {
    {CampoMensaje::TYPE, TipoMensaje::TEXT},
    {CampoMensaje::USERNAME, "Alice"},
  };
  
  EXPECT_THROW(GeneraMensaje::genera(json), std::invalid_argument);
}

/**
 * @brief Prueba el metodo generaRoomname() de la clase GeneraMensaje.
 * Comprueba que se obtenga el roomname del JSON correctamente.
 */
TEST(GeneraMensajeTest, GeneraRoomname){
  json json = {
    {CampoMensaje::TYPE, TipoMensaje::NEW_ROOM},
    {CampoMensaje::ROOMNAME, "Sala 1"}
  };
  
  Mensaje mensaje = GeneraMensaje::genera(json);

  EXPECT_TRUE(mensaje.getRoomname().has_value());
  EXPECT_EQ(mensaje.getRoomname().value(), "Sala 1");

  json = {
    {CampoMensaje::TYPE, TipoMensaje::NEW_ROOM}
  };
  
  EXPECT_THROW(GeneraMensaje::genera(json), std::invalid_argument);
}

/**
 * @brief Prueba el metodo generaUsernames() de la clase GeneraMensaje.
 * Comprueba que se obtenga los usernames del JSON correctamente.
 */
TEST(GeneraMensajeTest, GeneraUsernames){
  std::list<std::string> nombres = {"Alice", "Bob", "Charlie"};
  
  json json = {
    {CampoMensaje::TYPE, TipoMensaje::INVITE},
    {CampoMensaje::ROOMNAME, "Sala 1"},
    {CampoMensaje::USERNAMES, nombres}
  };
  
  Mensaje mensaje = GeneraMensaje::genera(json);

  EXPECT_TRUE(mensaje.getUsernames().has_value());
  EXPECT_EQ(mensaje.getUsernames().value(), nombres);

  json = {
    {CampoMensaje::TYPE, TipoMensaje::INVITE},
    {CampoMensaje::ROOMNAME, "Sala 1"}
  };
  
  EXPECT_THROW(GeneraMensaje::genera(json), std::invalid_argument);
}

/**
 * @brief Prueba el metodo genera() de la clase GeneraMensaje.
 * Comprueba que se obtenga los campos del JSON correctamente.
 */
TEST(GeneraMensajeTest, Genera){
  json json = {
    {CampoMensaje::TYPE, TipoMensaje::RESPONSE},
    {CampoMensaje::OPERATION, OperacionMensaje::IDENTIFY},
    {CampoMensaje::RESULT, ResultadoMensaje::SUCCESS},
    {CampoMensaje::EXTRA, "Alice"}
  };
  
  Mensaje mensaje = GeneraMensaje::genera(json);

  EXPECT_EQ(mensaje.getTipo(), TipoMensaje::RESPONSE);

  EXPECT_TRUE(mensaje.getOperacion().has_value());
  EXPECT_EQ(mensaje.getOperacion().value(), OperacionMensaje::IDENTIFY);

  EXPECT_TRUE(mensaje.getResultado().has_value());
  EXPECT_EQ(mensaje.getResultado().value(), ResultadoMensaje::SUCCESS);

  EXPECT_TRUE(mensaje.getExtra().has_value());
  EXPECT_EQ(mensaje.getExtra().value(), "Alice");

  json = {
    {CampoMensaje::OPERATION, OperacionMensaje::IDENTIFY},
    {CampoMensaje::RESULT, ResultadoMensaje::SUCCESS},
    {CampoMensaje::EXTRA, "Alice"}
  };
  
  EXPECT_THROW(GeneraMensaje::genera(json), std::invalid_argument);
}
