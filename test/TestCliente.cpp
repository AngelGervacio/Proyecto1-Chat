/**
 * @file TestCliente.cpp
 * @brief Pruebas unitarias de la clase Cliente.
 */

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <list>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <memory>
#include <cstdlib>
#include <string>
#include <unordered_map>
#include <mutex>
#include <iostream>
#include <thread>
#include <chrono>
#include "Conexion.hpp"
#include "Mensaje.hpp"
#include "Usuario.hpp"
#include "TipoMensaje.hpp"
#include "OperacionMensaje.hpp"
#include "ResultadoMensaje.hpp"
#include "GeneraJSON.hpp"
#include "GeneraMensaje.hpp"
#include "EstatusUsuario.hpp"
#include "Servidor.hpp"
#include "Cliente.hpp"
#include "Util.hpp"

using json = nlohmann::json;

/**
 * @brief Prueba que el cliente se pueda identificar.
 */
TEST(ClienteTest, Identificarse){
  std::thread hiloServidor;
  
  int puerto = puertoAleatorio();
  char datos[1024];
  
  Servidor servidor(puerto);

  hiloServidor = std::thread([&](){
    servidor.sirve();
  });

  int socketCnx = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in direccion;
  direccion.sin_family = AF_INET;
  direccion.sin_addr.s_addr = inet_addr("127.0.0.1");
  direccion.sin_port = htons(puerto);

  connect(socketCnx, (sockaddr*)&direccion, sizeof(direccion));
    
  json jsonAlice = GeneraJSON::genera(Mensaje::Builder()
				      .setTipo(TipoMensaje::IDENTIFY)
				      .setUsername("Alice")
				      .build());
    
  std::string lineaAlice = jsonAlice.dump() + "\n";
  send(socketCnx, lineaAlice.data(), lineaAlice.size(), 0);
  read(socketCnx, datos, sizeof(datos));

  Cliente cliente("127.0.0.1", puerto);
  cliente.identificarse("Charlie");

  auto bytesLeidos = read(socketCnx, datos, sizeof(datos));
  std::string linea(datos, bytesLeidos);
  json jsonRespuesta = json::parse(linea);

  json jsonEsperado = GeneraJSON::genera(Mensaje::Builder()
					 .setTipo(TipoMensaje::NEW_USER)
					 .setUsername("Charlie")
					 .build());

  EXPECT_EQ(jsonEsperado, jsonRespuesta);

  json solicitaUsuarios = GeneraJSON::genera(Mensaje::Builder()
					     .setTipo(TipoMensaje::USERS)
					     .build());

  std::string lineaUsuarios = solicitaUsuarios.dump() + "\n";
  send(socketCnx, lineaUsuarios.data(), lineaUsuarios.size(), 0);
  bytesLeidos = read(socketCnx, datos, sizeof(datos));
  linea = std::string(datos, bytesLeidos);
  jsonRespuesta = json::parse(linea);

  std::unordered_map<std::string, EstatusUsuario> usuarios = {{"Alice", EstatusUsuario::ACTIVE},
							      {"Charlie", EstatusUsuario::ACTIVE}};

  jsonEsperado = GeneraJSON::genera(Mensaje::Builder()
				    .setTipo(TipoMensaje::USER_LIST)
				    .setUsers(usuarios)
				    .build());

  EXPECT_EQ(jsonEsperado, jsonRespuesta);
  
  close(socketCnx);

  servidor.detenerServidor();
  
  hiloServidor.join();
}

/**
 * @brief Prueba que el cliente pueda cambiar el estatus.
 */
TEST(ClienteTest, CambiaEstatus){
  std::thread hiloServidor;
  
  int puerto = puertoAleatorio();
  char datos[1024];
  
  Servidor servidor(puerto);

  hiloServidor = std::thread([&](){
    servidor.sirve();
  });

  int socketCnx = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in direccion;
  direccion.sin_family = AF_INET;
  direccion.sin_addr.s_addr = inet_addr("127.0.0.1");
  direccion.sin_port = htons(puerto);

  connect(socketCnx, (sockaddr*)&direccion, sizeof(direccion));
    
  json jsonAlice = GeneraJSON::genera(Mensaje::Builder()
				      .setTipo(TipoMensaje::IDENTIFY)
				      .setUsername("Alice")
				      .build());
    
  std::string lineaAlice = jsonAlice.dump() + "\n";
  send(socketCnx, lineaAlice.data(), lineaAlice.size(), 0);
  read(socketCnx, datos, sizeof(datos));

  Cliente cliente("127.0.0.1", puerto);
  cliente.identificarse("Charlie");
  read(socketCnx, datos, sizeof(datos));

  cliente.cambiarEstatus(EstatusUsuario::BUSY);
  
  auto bytesLeidos = read(socketCnx, datos, sizeof(datos));
  std::string linea(datos, bytesLeidos);
  json jsonRespuesta = json::parse(linea);

  json jsonEsperado = GeneraJSON::genera(Mensaje::Builder()
					 .setTipo(TipoMensaje::NEW_STATUS)
					 .setUsername("Charlie")
					 .setEstatus(EstatusUsuario::BUSY)
					 .build());

  EXPECT_EQ(jsonEsperado, jsonRespuesta);
  
  close(socketCnx);

  servidor.detenerServidor();
  
  hiloServidor.join();
}

/**
 * @brief Prueba que el cliente pueda enviar un texto privado.
 */
TEST(ClienteTest, EnviaTexto){
  std::thread hiloServidor;
  
  int puerto = puertoAleatorio();
  char datos[1024];
  
  Servidor servidor(puerto);

  hiloServidor = std::thread([&](){
    servidor.sirve();
  });

  int socketCnx = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in direccion;
  direccion.sin_family = AF_INET;
  direccion.sin_addr.s_addr = inet_addr("127.0.0.1");
  direccion.sin_port = htons(puerto);

  connect(socketCnx, (sockaddr*)&direccion, sizeof(direccion));
    
  json jsonAlice = GeneraJSON::genera(Mensaje::Builder()
				      .setTipo(TipoMensaje::IDENTIFY)
				      .setUsername("Alice")
				      .build());
    
  std::string lineaAlice = jsonAlice.dump() + "\n";
  send(socketCnx, lineaAlice.data(), lineaAlice.size(), 0);
  read(socketCnx, datos, sizeof(datos));

  Cliente cliente("127.0.0.1", puerto);
  cliente.identificarse("Charlie");
  read(socketCnx, datos, sizeof(datos));

  cliente.enviarTexto("Alice", "Texto de prueba");
  
  auto bytesLeidos = read(socketCnx, datos, sizeof(datos));
  std::string linea(datos, bytesLeidos);
  json jsonRespuesta = json::parse(linea);

  json jsonEsperado = GeneraJSON::genera(Mensaje::Builder()
					 .setTipo(TipoMensaje::TEXT_FROM)
					 .setUsername("Charlie")
					 .setText("Texto de prueba")
					 .build());

  EXPECT_EQ(jsonEsperado, jsonRespuesta);
  
  close(socketCnx);

  servidor.detenerServidor();
  
  hiloServidor.join();
}

/**
 * @brief Prueba que el cliente pueda enviar un texto publico.
 */
TEST(ClienteTest, EnviaTextoPublico){
  std::thread hiloServidor;
  
  int puerto = puertoAleatorio();
  char datos[1024];
  
  Servidor servidor(puerto);

  hiloServidor = std::thread([&](){
    servidor.sirve();
  });

  int socketCnx = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in direccion;
  direccion.sin_family = AF_INET;
  direccion.sin_addr.s_addr = inet_addr("127.0.0.1");
  direccion.sin_port = htons(puerto);

  connect(socketCnx, (sockaddr*)&direccion, sizeof(direccion));
    
  json jsonAlice = GeneraJSON::genera(Mensaje::Builder()
				      .setTipo(TipoMensaje::IDENTIFY)
				      .setUsername("Alice")
				      .build());
    
  std::string lineaAlice = jsonAlice.dump() + "\n";
  send(socketCnx, lineaAlice.data(), lineaAlice.size(), 0);
  read(socketCnx, datos, sizeof(datos));

  Cliente cliente("127.0.0.1", puerto);
  cliente.identificarse("Charlie");
  read(socketCnx, datos, sizeof(datos));

  cliente.enviarTextoPublico("Texto de prueba publico");
  
  auto bytesLeidos = read(socketCnx, datos, sizeof(datos));
  std::string linea(datos, bytesLeidos);
  json jsonRespuesta = json::parse(linea);

  json jsonEsperado = GeneraJSON::genera(Mensaje::Builder()
					 .setTipo(TipoMensaje::PUBLIC_TEXT_FROM)
					 .setUsername("Charlie")
					 .setText("Texto de prueba publico")
					 .build());

  EXPECT_EQ(jsonEsperado, jsonRespuesta);
  
  close(socketCnx);

  servidor.detenerServidor();
  
  hiloServidor.join();
}

/**
 * @brief Prueba que el cliente pueda invitar usuarios a una sala.
 */
TEST(ClienteTest, InvitaUsuariosSala){
  std::thread hiloServidor;
  
  int puerto = puertoAleatorio();
  char datos[1024];
  
  Servidor servidor(puerto);

  hiloServidor = std::thread([&](){
    servidor.sirve();
  });

  int socketCnx = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in direccion;
  direccion.sin_family = AF_INET;
  direccion.sin_addr.s_addr = inet_addr("127.0.0.1");
  direccion.sin_port = htons(puerto);

  connect(socketCnx, (sockaddr*)&direccion, sizeof(direccion));
    
  json jsonAlice = GeneraJSON::genera(Mensaje::Builder()
				      .setTipo(TipoMensaje::IDENTIFY)
				      .setUsername("Alice")
				      .build());
    
  std::string lineaAlice = jsonAlice.dump() + "\n";
  send(socketCnx, lineaAlice.data(), lineaAlice.size(), 0);
  read(socketCnx, datos, sizeof(datos));

  Cliente cliente("127.0.0.1", puerto);
  cliente.identificarse("Charlie");
  read(socketCnx, datos, sizeof(datos));

  cliente.crearSala("Sala 1");
  cliente.invitar({"Alice"}, "Sala 1");
  
  auto bytesLeidos = read(socketCnx, datos, sizeof(datos));
  std::string linea(datos, bytesLeidos);
  json jsonRespuesta = json::parse(linea);

  json jsonEsperado = GeneraJSON::genera(Mensaje::Builder()
					 .setTipo(TipoMensaje::INVITATION)
					 .setUsername("Charlie")
					 .setRoomname("Sala 1")
					 .build());

  EXPECT_EQ(jsonEsperado, jsonRespuesta);
  
  close(socketCnx);

  servidor.detenerServidor();
  
  hiloServidor.join();
}

/**
 * @brief Prueba que el cliente pueda entrar a una sala.
 */
TEST(ClienteTest, EntraSala){
  std::thread hiloServidor;
  
  int puerto = puertoAleatorio();
  char datos[1024];
  
  Servidor servidor(puerto);

  hiloServidor = std::thread([&](){
    servidor.sirve();
  });

  int socketCnx = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in direccion;
  direccion.sin_family = AF_INET;
  direccion.sin_addr.s_addr = inet_addr("127.0.0.1");
  direccion.sin_port = htons(puerto);

  connect(socketCnx, (sockaddr*)&direccion, sizeof(direccion));
    
  json jsonAlice = GeneraJSON::genera(Mensaje::Builder()
				      .setTipo(TipoMensaje::IDENTIFY)
				      .setUsername("Alice")
				      .build());
    
  std::string lineaAlice = jsonAlice.dump() + "\n";
  send(socketCnx, lineaAlice.data(), lineaAlice.size(), 0);
  read(socketCnx, datos, sizeof(datos));

  Cliente cliente("127.0.0.1", puerto);
  cliente.identificarse("Charlie");
  read(socketCnx, datos, sizeof(datos));

  json creaSala = GeneraJSON::genera(Mensaje::Builder()
				     .setTipo(TipoMensaje::NEW_ROOM)
				     .setRoomname("Sala 1")
				     .build());

  std::string lineaSala = creaSala.dump() + "\n";
  send(socketCnx, lineaSala.data(), lineaSala.size(), 0);
  read(socketCnx, datos, sizeof(datos));

  json invita = GeneraJSON::genera(Mensaje::Builder()
				   .setTipo(TipoMensaje::INVITE)
				   .setRoomname("Sala 1")
				   .setUsernames({"Charlie"})
				   .build());

  std::string lineaInvita = invita.dump() + "\n";
  send(socketCnx, lineaInvita.data(), lineaInvita.size(), 0);

  std::this_thread::sleep_for (std::chrono::milliseconds(10));

  cliente.entrarSala("Sala 1");
  
  auto bytesLeidos = read(socketCnx, datos, sizeof(datos));
  std::string linea(datos, bytesLeidos);
  json jsonRespuesta = json::parse(linea);

  json jsonEsperado = GeneraJSON::genera(Mensaje::Builder()
					 .setTipo(TipoMensaje::JOINED_ROOM)
					 .setRoomname("Sala 1")
					 .setUsername("Charlie")
					 .build());

  EXPECT_EQ(jsonEsperado, jsonRespuesta);
  
  close(socketCnx);

  servidor.detenerServidor();
  
  hiloServidor.join();
}

/**
 * @brief Prueba que el cliente pueda enviar texto a una sala.
 */
TEST(ClienteTest, EnviaTextoSala){
  std::thread hiloServidor;
  
  int puerto = puertoAleatorio();
  char datos[1024];
  
  Servidor servidor(puerto);

  hiloServidor = std::thread([&](){
    servidor.sirve();
  });

  int socketCnx = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in direccion;
  direccion.sin_family = AF_INET;
  direccion.sin_addr.s_addr = inet_addr("127.0.0.1");
  direccion.sin_port = htons(puerto);

  connect(socketCnx, (sockaddr*)&direccion, sizeof(direccion));
    
  json jsonAlice = GeneraJSON::genera(Mensaje::Builder()
				      .setTipo(TipoMensaje::IDENTIFY)
				      .setUsername("Alice")
				      .build());
    
  std::string lineaAlice = jsonAlice.dump() + "\n";
  send(socketCnx, lineaAlice.data(), lineaAlice.size(), 0);
  read(socketCnx, datos, sizeof(datos));

  Cliente cliente("127.0.0.1", puerto);
  cliente.identificarse("Charlie");
  read(socketCnx, datos, sizeof(datos));

  json creaSala = GeneraJSON::genera(Mensaje::Builder()
				     .setTipo(TipoMensaje::NEW_ROOM)
				     .setRoomname("Sala 1")
				     .build());

  std::string lineaSala = creaSala.dump() + "\n";
  send(socketCnx, lineaSala.data(), lineaSala.size(), 0);
  read(socketCnx, datos, sizeof(datos));

  json invita = GeneraJSON::genera(Mensaje::Builder()
				   .setTipo(TipoMensaje::INVITE)
				   .setRoomname("Sala 1")
				   .setUsernames({"Charlie"})
				   .build());

  std::string lineaInvita = invita.dump() + "\n";
  send(socketCnx, lineaInvita.data(), lineaInvita.size(), 0);

  std::this_thread::sleep_for (std::chrono::milliseconds(10));

  cliente.entrarSala("Sala 1");

  read(socketCnx, datos, sizeof(datos));

  cliente.enviarTextoSala("Sala 1", "Texto de prueba sala");
  
  auto bytesLeidos = read(socketCnx, datos, sizeof(datos));
  std::string linea(datos, bytesLeidos);
  json jsonRespuesta = json::parse(linea);

  json jsonEsperado = GeneraJSON::genera(Mensaje::Builder()
					 .setTipo(TipoMensaje::ROOM_TEXT_FROM)
					 .setRoomname("Sala 1")
					 .setUsername("Charlie")
					 .setText("Texto de prueba sala")
					 .build());

  EXPECT_EQ(jsonEsperado, jsonRespuesta);
  
  close(socketCnx);

  servidor.detenerServidor();
  
  hiloServidor.join();
}

/**
 * @brief Prueba que el cliente pueda salir de una sala.
 */
TEST(ClienteTest, SaleSala){
  std::thread hiloServidor;
  
  int puerto = puertoAleatorio();
  char datos[1024];
  
  Servidor servidor(puerto);

  hiloServidor = std::thread([&](){
    servidor.sirve();
  });

  int socketCnx = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in direccion;
  direccion.sin_family = AF_INET;
  direccion.sin_addr.s_addr = inet_addr("127.0.0.1");
  direccion.sin_port = htons(puerto);

  connect(socketCnx, (sockaddr*)&direccion, sizeof(direccion));
    
  json jsonAlice = GeneraJSON::genera(Mensaje::Builder()
				      .setTipo(TipoMensaje::IDENTIFY)
				      .setUsername("Alice")
				      .build());
    
  std::string lineaAlice = jsonAlice.dump() + "\n";
  send(socketCnx, lineaAlice.data(), lineaAlice.size(), 0);
  read(socketCnx, datos, sizeof(datos));

  Cliente cliente("127.0.0.1", puerto);
  cliente.identificarse("Charlie");
  read(socketCnx, datos, sizeof(datos));

  json creaSala = GeneraJSON::genera(Mensaje::Builder()
				     .setTipo(TipoMensaje::NEW_ROOM)
				     .setRoomname("Sala 1")
				     .build());

  std::string lineaSala = creaSala.dump() + "\n";
  send(socketCnx, lineaSala.data(), lineaSala.size(), 0);
  read(socketCnx, datos, sizeof(datos));

  json invita = GeneraJSON::genera(Mensaje::Builder()
				   .setTipo(TipoMensaje::INVITE)
				   .setRoomname("Sala 1")
				   .setUsernames({"Charlie"})
				   .build());

  std::string lineaInvita = invita.dump() + "\n";
  send(socketCnx, lineaInvita.data(), lineaInvita.size(), 0);

  std::this_thread::sleep_for (std::chrono::milliseconds(10));

  cliente.entrarSala("Sala 1");

  read(socketCnx, datos, sizeof(datos));

  cliente.salirSala("Sala 1");
  
  auto bytesLeidos = read(socketCnx, datos, sizeof(datos));
  std::string linea(datos, bytesLeidos);
  json jsonRespuesta = json::parse(linea);

  json jsonEsperado = GeneraJSON::genera(Mensaje::Builder()
					 .setTipo(TipoMensaje::LEFT_ROOM)
					 .setRoomname("Sala 1")
					 .setUsername("Charlie")
					 .build());

  EXPECT_EQ(jsonEsperado, jsonRespuesta);
  
  close(socketCnx);

  servidor.detenerServidor();
  
  hiloServidor.join();
}

/**
 * @brief Prueba que el cliente se pueda desconectar.
 */
TEST(ClienteTest, Desconecta){
  std::thread hiloServidor;
  
  int puerto = puertoAleatorio();
  char datos[1024];
  
  Servidor servidor(puerto);

  hiloServidor = std::thread([&](){
    servidor.sirve();
  });

  int socketCnx = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in direccion;
  direccion.sin_family = AF_INET;
  direccion.sin_addr.s_addr = inet_addr("127.0.0.1");
  direccion.sin_port = htons(puerto);

  connect(socketCnx, (sockaddr*)&direccion, sizeof(direccion));
    
  json jsonAlice = GeneraJSON::genera(Mensaje::Builder()
				      .setTipo(TipoMensaje::IDENTIFY)
				      .setUsername("Alice")
				      .build());
    
  std::string lineaAlice = jsonAlice.dump() + "\n";
  send(socketCnx, lineaAlice.data(), lineaAlice.size(), 0);
  read(socketCnx, datos, sizeof(datos));

  Cliente cliente("127.0.0.1", puerto);
  cliente.identificarse("Charlie");
  read(socketCnx, datos, sizeof(datos));

  cliente.desconectar();
  
  auto bytesLeidos = read(socketCnx, datos, sizeof(datos));
  std::string linea(datos, bytesLeidos);
  json jsonRespuesta = json::parse(linea);

  json jsonEsperado = GeneraJSON::genera(Mensaje::Builder()
					 .setTipo(TipoMensaje::DISCONNECTED)
					 .setUsername("Charlie")
					 .build());

  EXPECT_EQ(jsonEsperado, jsonRespuesta);
  
  close(socketCnx);

  servidor.detenerServidor();
  
  hiloServidor.join();
}
