/**
 * @file TestGeneraJSON.cpp
 * @brief Pruebas unitarias de la clase GeneraJSON.
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
#include "GeneraJSON.hpp"

using json = nlohmann::json;

TEST(GeneraJSONTest, GeneraTipo){
  Mensaje mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::IDENTIFY)
    .setUsername("Alice")
    .build();

  json json = GeneraJSON::genera(mensaje);

  EXPECT_TRUE(json.contains(CampoMensaje::TYPE));
  EXPECT_EQ(json[CampoMensaje::TYPE].get<TipoMensaje>(), TipoMensaje::IDENTIFY);

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::NO_VALIDO)
    .build();

  EXPECT_THROW(GeneraJSON::genera(mensaje), std::invalid_argument);
}

TEST(GeneraJSONTest, GeneraOperacion){
  Mensaje mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::RESPONSE)
    .setOperacion(OperacionMensaje::IDENTIFY)
    .setResultado(ResultadoMensaje::SUCCESS)
    .setExtra("Alice")
    .build();

  json json = GeneraJSON::genera(mensaje);

  EXPECT_TRUE(json.contains(CampoMensaje::OPERATION));
  EXPECT_EQ(json[CampoMensaje::OPERATION].get<OperacionMensaje>(), OperacionMensaje::IDENTIFY);

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::RESPONSE)
    .setResultado(ResultadoMensaje::SUCCESS)
    .setExtra("Alice")
    .build();

  EXPECT_THROW(GeneraJSON::genera(mensaje), std::invalid_argument);

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::RESPONSE)
    .setOperacion(OperacionMensaje::NO_VALIDO)
    .setResultado(ResultadoMensaje::SUCCESS)
    .setExtra("Alice")
    .build();

  EXPECT_THROW(GeneraJSON::genera(mensaje), std::invalid_argument);
}

TEST(GeneraJSONTest, GeneraResultado){
  Mensaje mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::RESPONSE)
    .setOperacion(OperacionMensaje::IDENTIFY)
    .setResultado(ResultadoMensaje::SUCCESS)
    .setExtra("Alice")
    .build();

  json json = GeneraJSON::genera(mensaje);

  EXPECT_TRUE(json.contains(CampoMensaje::RESULT));
  EXPECT_EQ(json[CampoMensaje::RESULT].get<ResultadoMensaje>(), ResultadoMensaje::SUCCESS);

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::RESPONSE)
    .setOperacion(OperacionMensaje::IDENTIFY)
    .setExtra("Alice")
    .build();

  EXPECT_THROW(GeneraJSON::genera(mensaje), std::invalid_argument);

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::RESPONSE)
    .setOperacion(OperacionMensaje::IDENTIFY)
    .setResultado(ResultadoMensaje::NO_VALIDO)
    .setExtra("Alice")
    .build();

  EXPECT_THROW(GeneraJSON::genera(mensaje), std::invalid_argument);
}

TEST(GeneraJSONTest, GeneraUsername){
  Mensaje mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::IDENTIFY)
    .setUsername("Alice")
    .build();

  json json = GeneraJSON::genera(mensaje);

  EXPECT_TRUE(json.contains(CampoMensaje::USERNAME));
  EXPECT_EQ(json[CampoMensaje::USERNAME].get<std::string>(), "Alice");

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::IDENTIFY)
    .build();

  EXPECT_THROW(GeneraJSON::genera(mensaje), std::invalid_argument);
}

TEST(GeneraJSONTest, GeneraExtra){
  Mensaje mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::RESPONSE)
    .setOperacion(OperacionMensaje::IDENTIFY)
    .setResultado(ResultadoMensaje::SUCCESS)
    .setExtra("Alice")
    .build();

  json json = GeneraJSON::genera(mensaje);

  EXPECT_TRUE(json.contains(CampoMensaje::EXTRA));
  EXPECT_EQ(json[CampoMensaje::EXTRA].get<std::string>(), "Alice");

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::RESPONSE)
    .setOperacion(OperacionMensaje::IDENTIFY)
    .setResultado(ResultadoMensaje::SUCCESS)
    .build();

  EXPECT_THROW(GeneraJSON::genera(mensaje), std::invalid_argument);
}

TEST(GeneraJSONTest, GeneraEstatus){
  Mensaje mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::STATUS)
    .setEstatus(EstatusUsuario::ACTIVE)
    .build();

  json json = GeneraJSON::genera(mensaje);

  EXPECT_TRUE(json.contains(CampoMensaje::STATUS));
  EXPECT_EQ(json[CampoMensaje::STATUS].get<EstatusUsuario>(), EstatusUsuario::ACTIVE);

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::STATUS)
    .setEstatus(EstatusUsuario::NO_VALIDO)
    .build();

  EXPECT_THROW(GeneraJSON::genera(mensaje), std::invalid_argument);

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::STATUS)
    .build();
  
  EXPECT_THROW(GeneraJSON::genera(mensaje), std::invalid_argument);
}

TEST(GeneraJSONTest, GeneraUsers){
  std::unordered_map<std::string, EstatusUsuario> users = {
    {"Alice", EstatusUsuario::ACTIVE},
    {"Bob", EstatusUsuario::AWAY},
    {"Charlie", EstatusUsuario::BUSY}
  };
  
  Mensaje mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::USER_LIST)
    .setUsers(users)
    .build();

  json json = GeneraJSON::genera(mensaje);

  EXPECT_TRUE(json.contains(CampoMensaje::USERS));

  auto usersjson = json[CampoMensaje::USERS].get<std::unordered_map<std::string, EstatusUsuario>>();
  
  EXPECT_EQ(usersjson, users);

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::USER_LIST)
    .build();

  EXPECT_THROW(GeneraJSON::genera(mensaje), std::invalid_argument);

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::USER_LIST)
    .setUsers({{"Alice", EstatusUsuario::NO_VALIDO}})
    .build();
  
  EXPECT_THROW(GeneraJSON::genera(mensaje), std::invalid_argument);
}

TEST(GeneraJSONTest, GeneraText){
  Mensaje mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::TEXT)
    .setUsername("Alice")
    .setText("Texto necesario")
    .build();

  json json = GeneraJSON::genera(mensaje);

  EXPECT_TRUE(json.contains(CampoMensaje::TEXT));
  EXPECT_EQ(json[CampoMensaje::TEXT].get<std::string>(), "Texto necesario");

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::TEXT)
    .setUsername("Alice")
    .build();

  EXPECT_THROW(GeneraJSON::genera(mensaje), std::invalid_argument);
}

TEST(GeneraJSONTest, GeneraRoomname){
  Mensaje mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::NEW_ROOM)
    .setRoomname("Sala 1")
    .build();

  json json = GeneraJSON::genera(mensaje);

  EXPECT_TRUE(json.contains(CampoMensaje::ROOMNAME));
  EXPECT_EQ(json[CampoMensaje::ROOMNAME].get<std::string>(), "Sala 1");

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::NEW_ROOM)
    .build();

  EXPECT_THROW(GeneraJSON::genera(mensaje), std::invalid_argument);
}

TEST(GeneraJSONTest, GeneraUsernames){
  std::list<std::string> nombres = {"Alice", "Bob", "Charlie"};
  
  Mensaje mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::INVITE)
    .setRoomname("Sala 1")
    .setUsernames(nombres)
    .build();

  json json = GeneraJSON::genera(mensaje);

  EXPECT_TRUE(json.contains(CampoMensaje::USERNAMES));
  EXPECT_EQ(json[CampoMensaje::USERNAMES].get<std::list<std::string>>(), nombres);

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::INVITE)
    .setRoomname("Sala 1")
    .build();

  EXPECT_THROW(GeneraJSON::genera(mensaje), std::invalid_argument);
}

TEST(GeneraJSONTest, Genera){
  Mensaje mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::RESPONSE)
    .setOperacion(OperacionMensaje::IDENTIFY)
    .setResultado(ResultadoMensaje::SUCCESS)
    .setExtra("Alice")
    .build();

  json json = GeneraJSON::genera(mensaje);

  EXPECT_TRUE(json.contains(CampoMensaje::TYPE));
  EXPECT_EQ(json[CampoMensaje::TYPE].get<TipoMensaje>(), TipoMensaje::RESPONSE);

  EXPECT_TRUE(json.contains(CampoMensaje::OPERATION));
  EXPECT_EQ(json[CampoMensaje::OPERATION].get<OperacionMensaje>(), OperacionMensaje::IDENTIFY);
  
  EXPECT_TRUE(json.contains(CampoMensaje::RESULT));
  EXPECT_EQ(json[CampoMensaje::RESULT].get<ResultadoMensaje>(), ResultadoMensaje::SUCCESS);
  
  EXPECT_TRUE(json.contains(CampoMensaje::EXTRA));
  EXPECT_EQ(json[CampoMensaje::EXTRA].get<std::string>(), "Alice");

  mensaje = Mensaje::Builder()
    .setOperacion(OperacionMensaje::IDENTIFY)
    .setResultado(ResultadoMensaje::SUCCESS)
    .setExtra("Alice")
    .build();

  EXPECT_THROW(GeneraJSON::genera(mensaje), std::invalid_argument);
}
