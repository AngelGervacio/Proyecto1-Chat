/**
 * @file TestServidor.cpp
 * @brief Pruebas unitarias de la clase Servidor.
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
#include <random>
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

using json = nlohmann::json;

/**
 * @brief Regresa un puerto disponible.
 * @return El puerto disponible.
 */
int puertoAleatorio(){
  int puerto = 0;
  
    while(puerto < 1024){
      int socketPrueba = socket(AF_INET, SOCK_STREAM, 0);
  
      sockaddr_in direccion{};
      direccion.sin_family = AF_INET;
      direccion.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
      direccion.sin_port = htons(0);

      if(bind(socketPrueba, (sockaddr*)&direccion, sizeof(direccion)) < 0){
	close(socketPrueba);
	puerto = -1;
	std::this_thread::sleep_for (std::chrono::milliseconds(10));
	continue;
      }

      socklen_t longitud = sizeof(direccion);
      getsockname(socketPrueba, (sockaddr*)&direccion, &longitud);

      puerto = ntohs(direccion.sin_port);

      close(socketPrueba);
    }

  return puerto;
}

/**
 * @brief Prueba que se identifique correctame a una conexión.
 */
TEST(ServidorTest, Identifica){
  std::thread hiloServidor;
  
  int puerto = puertoAleatorio();
  char datos[1024];
  
  Servidor servidor(puerto);

  hiloServidor = std::thread([&](){
    servidor.sirve();
  });

  int socketCnx1 = socket(AF_INET, SOCK_STREAM, 0);
  int socketCnx2 = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in direccion;
  direccion.sin_family = AF_INET;
  direccion.sin_addr.s_addr = inet_addr("127.0.0.1");
  direccion.sin_port = htons(puerto);

  connect(socketCnx1, (sockaddr*)&direccion, sizeof(direccion));
  connect(socketCnx2, (sockaddr*)&direccion, sizeof(direccion));
    
  json jsonAlice = GeneraJSON::genera(Mensaje::Builder()
				      .setTipo(TipoMensaje::IDENTIFY)
				      .setUsername("Alice")
				      .build());
    
  json jsonCharlie = GeneraJSON::genera(Mensaje::Builder()
					.setTipo(TipoMensaje::IDENTIFY)
					.setUsername("Charlie")
					.build());
    
  std::string lineaAlice = jsonAlice.dump() + "\n";
  std::string lineaCharlie = jsonCharlie.dump() + "\n";

  send(socketCnx1, lineaAlice.data(), lineaAlice.size(), 0);
    
  json respuestaAlice = GeneraJSON::genera(Mensaje::Builder()
					   .setTipo(TipoMensaje::RESPONSE)
					   .setOperacion(OperacionMensaje::IDENTIFY)
					   .setResultado(ResultadoMensaje::SUCCESS)
					   .setExtra("Alice")
					   .build());

  auto bytesLeidos = read(socketCnx1, datos, sizeof(datos));
  std::string linea(datos, bytesLeidos);
  json jsonRespuesta = json::parse(linea);

  EXPECT_EQ(respuestaAlice, jsonRespuesta);

  send(socketCnx2, lineaAlice.data(), lineaAlice.size(), 0);

  json jsonYaExiste = GeneraJSON::genera(Mensaje::Builder()
					 .setTipo(TipoMensaje::RESPONSE)
					 .setOperacion(OperacionMensaje::IDENTIFY)
					 .setResultado(ResultadoMensaje::USER_ALREADY_EXISTS)
					 .setExtra("Alice")
					 .build());
    
  bytesLeidos = read(socketCnx2, datos, sizeof(datos));
  linea = std::string(datos, bytesLeidos);
  jsonRespuesta = json::parse(linea);

  EXPECT_EQ(jsonYaExiste, jsonRespuesta);

  send(socketCnx2, lineaCharlie.data(), lineaCharlie.size(), 0);

  json respuestaCharlie = GeneraJSON::genera(Mensaje::Builder()
					     .setTipo(TipoMensaje::RESPONSE)
					     .setOperacion(OperacionMensaje::IDENTIFY)
					     .setResultado(ResultadoMensaje::SUCCESS)
					     .setExtra("Charlie")
					     .build());
    
  bytesLeidos = read(socketCnx2, datos, sizeof(datos));
  linea = std::string(datos, bytesLeidos);
  jsonRespuesta = json::parse(linea);

  EXPECT_EQ(respuestaCharlie, jsonRespuesta);

  bytesLeidos = read(socketCnx1, datos, sizeof(datos));
  linea = std::string(datos, bytesLeidos);
  jsonRespuesta = json::parse(linea);

  json nuevoUsuario = GeneraJSON::genera(Mensaje::Builder()
					 .setTipo(TipoMensaje::NEW_USER)
					 .setUsername("Charlie")
					 .build());

  EXPECT_EQ(nuevoUsuario, jsonRespuesta);
    
  close(socketCnx1);
  close(socketCnx2);

  servidor.detenerServidor();
  
  hiloServidor.join();
}

/**
 * @brief Prueba que se envie correctame la lista de usuarios.
 */
TEST(ServidorTest, RecibeListaUsuarios){
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
    
  json jsonIdentify = GeneraJSON::genera(Mensaje::Builder()
					 .setTipo(TipoMensaje::IDENTIFY)
					 .setUsername("Alice")
					 .build());
    
  std::string lineaJson = jsonIdentify.dump() + "\n";

  send(socketCnx, lineaJson.data(), lineaJson.size(), 0);

  auto bytesLeidos = read(socketCnx, datos, sizeof(datos));
    
  json jsonSolicita = GeneraJSON::genera(Mensaje::Builder()
					 .setTipo(TipoMensaje::USERS)
					 .build());
    
  std::string lineaSolicita = jsonSolicita.dump() + "\n";

  send(socketCnx, lineaSolicita.data(), lineaSolicita.size(), 0);

  std::unordered_map<std::string, EstatusUsuario> usuarios = {{"Alice", EstatusUsuario::ACTIVE}};
    
  json jsonEsperado = GeneraJSON::genera(Mensaje::Builder()
					 .setTipo(TipoMensaje::USER_LIST)
					 .setUsers(usuarios)
					 .build());
    
  bytesLeidos = read(socketCnx, datos, sizeof(datos));
  std::string linea(datos, bytesLeidos);
  json jsonRespuesta = json::parse(linea);

  EXPECT_EQ(jsonEsperado, jsonRespuesta);
  
  close(socketCnx);

  servidor.detenerServidor();
  hiloServidor.join();
}

/**
 * @brief Prueba que se cambie correctamen el estatus de una conexión.
 */
TEST(ServidorTest, CambiaEstatus){
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

  json jsonIdentify = GeneraJSON::genera(Mensaje::Builder()
					 .setTipo(TipoMensaje::IDENTIFY)
					 .setUsername("Alice")
					 .build());
    
  std::string lineaJson = jsonIdentify.dump() + "\n";

  send(socketCnx, lineaJson.data(), lineaJson.size(), 0);

  auto bytesLeidos = read(socketCnx, datos, sizeof(datos));

  json jsonSolicita = GeneraJSON::genera(Mensaje::Builder()
					 .setTipo(TipoMensaje::USERS)
					 .build());
    
  std::string lineaSolicita = jsonSolicita.dump() + "\n";

  send(socketCnx, lineaSolicita.data(), lineaSolicita.size(), 0);

  std::unordered_map<std::string, EstatusUsuario> usuarios = {{"Alice", EstatusUsuario::ACTIVE}};
  
  json jsonEsperado = GeneraJSON::genera(Mensaje::Builder()
					 .setTipo(TipoMensaje::USER_LIST)
					 .setUsers(usuarios)
					 .build());
 
  bytesLeidos = read(socketCnx, datos, sizeof(datos));
  std::string linea(datos, bytesLeidos);
  json jsonRespuesta = json::parse(linea);

  Mensaje lista = GeneraMensaje::genera(jsonRespuesta);

  EXPECT_TRUE(lista.getUsers().has_value());
  EXPECT_EQ(lista.getUsers().value()["Alice"], EstatusUsuario::ACTIVE);
    
  json jsonCambia = GeneraJSON::genera(Mensaje::Builder()
				       .setTipo(TipoMensaje::STATUS)
				       .setEstatus(EstatusUsuario::BUSY)
				       .build());
    
  lineaJson = jsonCambia.dump() + "\n";

  send(socketCnx, lineaJson.data(), lineaJson.size(), 0);
  
  usuarios = {{"Alice", EstatusUsuario::BUSY}};

  send(socketCnx, lineaSolicita.data(), lineaSolicita.size(), 0);

  jsonEsperado = GeneraJSON::genera(Mensaje::Builder()
				    .setTipo(TipoMensaje::USER_LIST)
				    .setUsers(usuarios)
				    .build());
    
  bytesLeidos = read(socketCnx, datos, sizeof(datos));
  linea = std::string(datos, bytesLeidos);
  jsonRespuesta = json::parse(linea);
  
  lista = GeneraMensaje::genera(jsonRespuesta);

  EXPECT_TRUE(lista.getUsers().has_value());
  EXPECT_EQ(lista.getUsers().value()["Alice"], EstatusUsuario::BUSY);
  
  close(socketCnx);
    
  servidor.detenerServidor();
  hiloServidor.join();
}

/**
 * @brief Prueba que se envia correctame un texto privado.
 */
TEST(ServidorTest, EnviaTextoPrivado){
  std::thread hiloServidor;
  
  int puerto = puertoAleatorio();
  char datos[1024];
  
  Servidor servidor(puerto);

  hiloServidor = std::thread([&](){
    servidor.sirve();
  });

  int socketCnx1 = socket(AF_INET, SOCK_STREAM, 0);
  int socketCnx2 = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in direccion;
  direccion.sin_family = AF_INET;
  direccion.sin_addr.s_addr = inet_addr("127.0.0.1");
  direccion.sin_port = htons(puerto);

  connect(socketCnx1, (sockaddr*)&direccion, sizeof(direccion));
  connect(socketCnx2, (sockaddr*)&direccion, sizeof(direccion));
    
  json jsonAlice = GeneraJSON::genera(Mensaje::Builder()
				      .setTipo(TipoMensaje::IDENTIFY)
				      .setUsername("Alice")
				      .build());
    
  json jsonCharlie = GeneraJSON::genera(Mensaje::Builder()
					.setTipo(TipoMensaje::IDENTIFY)
					.setUsername("Charlie")
					.build());
    
  std::string lineaAlice = jsonAlice.dump() + "\n";
  std::string lineaCharlie = jsonCharlie.dump() + "\n";

  send(socketCnx1, lineaAlice.data(), lineaAlice.size(), 0);
  auto bytesLeidos = read(socketCnx1, datos, sizeof(datos));

  send(socketCnx2, lineaCharlie.data(), lineaCharlie.size(), 0);    
  bytesLeidos = read(socketCnx2, datos, sizeof(datos));

  bytesLeidos = read(socketCnx1, datos, sizeof(datos));

  json enviaTexto = GeneraJSON::genera(Mensaje::Builder()
				       .setTipo(TipoMensaje::TEXT)
				       .setUsername("Charlie")
				       .setText("Hola Charlie")
				       .build());

  std::string lineaTexto = enviaTexto.dump() + "\n";
  send(socketCnx1, lineaTexto.data(), lineaTexto.size(), 0);

  bytesLeidos = read(socketCnx2, datos, sizeof(datos));
  std::string linea(datos, bytesLeidos);
  json jsonRespuesta = json::parse(linea);

  json jsonEsperado = GeneraJSON::genera(Mensaje::Builder()
					 .setTipo(TipoMensaje::TEXT_FROM)
					 .setUsername("Alice")
					 .setText("Hola Charlie")
					 .build());

  EXPECT_EQ(jsonEsperado, jsonRespuesta);

  enviaTexto = GeneraJSON::genera(Mensaje::Builder()
				  .setTipo(TipoMensaje::TEXT)
				  .setUsername("Bob")
				  .setText("Hola Bob")
				  .build());

  lineaTexto = enviaTexto.dump() + "\n";
  send(socketCnx1, lineaTexto.data(), lineaTexto.size(), 0);

  bytesLeidos = read(socketCnx1, datos, sizeof(datos));
  linea = std::string(datos, bytesLeidos);
  jsonRespuesta = json::parse(linea);

  jsonEsperado = GeneraJSON::genera(Mensaje::Builder()
				    .setTipo(TipoMensaje::RESPONSE)
				    .setOperacion(OperacionMensaje::TEXT)
				    .setResultado(ResultadoMensaje::NO_SUCH_USER)
				    .setExtra("Bob")
				    .build());
  
  EXPECT_EQ(jsonEsperado, jsonRespuesta);
    
  close(socketCnx1);
  close(socketCnx2);

  servidor.detenerServidor();
  
  hiloServidor.join();
}

/**
 * @brief Prueba que se envie correctamente un texto publico.
 */
TEST(ServidorTest, EnviaTextoPublico){
  std::thread hiloServidor;
  
  int puerto = puertoAleatorio();
  char datos[1024];
  
  Servidor servidor(puerto);

  hiloServidor = std::thread([&](){
    servidor.sirve();
  });

  int socketCnx1 = socket(AF_INET, SOCK_STREAM, 0);
  int socketCnx2 = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in direccion;
  direccion.sin_family = AF_INET;
  direccion.sin_addr.s_addr = inet_addr("127.0.0.1");
  direccion.sin_port = htons(puerto);

  connect(socketCnx1, (sockaddr*)&direccion, sizeof(direccion));
  connect(socketCnx2, (sockaddr*)&direccion, sizeof(direccion));
    
  json jsonAlice = GeneraJSON::genera(Mensaje::Builder()
				      .setTipo(TipoMensaje::IDENTIFY)
				      .setUsername("Alice")
				      .build());
    
  json jsonCharlie = GeneraJSON::genera(Mensaje::Builder()
					.setTipo(TipoMensaje::IDENTIFY)
					.setUsername("Charlie")
					.build());
    
  std::string lineaAlice = jsonAlice.dump() + "\n";
  std::string lineaCharlie = jsonCharlie.dump() + "\n";

  send(socketCnx1, lineaAlice.data(), lineaAlice.size(), 0);
  auto bytesLeidos = read(socketCnx1, datos, sizeof(datos));

  send(socketCnx2, lineaCharlie.data(), lineaCharlie.size(), 0);    
  bytesLeidos = read(socketCnx2, datos, sizeof(datos));

  bytesLeidos = read(socketCnx1, datos, sizeof(datos));

  json enviaTextoPublico = GeneraJSON::genera(Mensaje::Builder()
				       .setTipo(TipoMensaje::PUBLIC_TEXT)
				       .setText("Hola a todos")
				       .build());

  std::string lineaTexto = enviaTextoPublico.dump() + "\n";
  send(socketCnx1, lineaTexto.data(), lineaTexto.size(), 0);

  bytesLeidos = read(socketCnx2, datos, sizeof(datos));
  std::string linea(datos, bytesLeidos);
  json jsonRespuesta = json::parse(linea);

  json jsonEsperado = GeneraJSON::genera(Mensaje::Builder()
					 .setTipo(TipoMensaje::PUBLIC_TEXT_FROM)
					 .setUsername("Alice")
					 .setText("Hola a todos")
					 .build());

  EXPECT_EQ(jsonEsperado, jsonRespuesta);
  
  close(socketCnx1);
  close(socketCnx2);

  servidor.detenerServidor();
  
  hiloServidor.join();
}

/**
 * @brief Prueba que se pueda crear una sala correctamente.
 */
TEST(ServidorTest, CreaSala){
  std::thread hiloServidor;
  
  int puerto = puertoAleatorio();
  char datos[1024];
  
  Servidor servidor(puerto);

  hiloServidor = std::thread([&](){
    servidor.sirve();
  });

  int socketCnx1 = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in direccion;
  direccion.sin_family = AF_INET;
  direccion.sin_addr.s_addr = inet_addr("127.0.0.1");
  direccion.sin_port = htons(puerto);

  connect(socketCnx1, (sockaddr*)&direccion, sizeof(direccion));
    
  json jsonAlice = GeneraJSON::genera(Mensaje::Builder()
				      .setTipo(TipoMensaje::IDENTIFY)
				      .setUsername("Alice")
				      .build());
    
  std::string lineaAlice = jsonAlice.dump() + "\n";

  send(socketCnx1, lineaAlice.data(), lineaAlice.size(), 0);
  auto bytesLeidos = read(socketCnx1, datos, sizeof(datos));

  json creaSala = GeneraJSON::genera(Mensaje::Builder()
				     .setTipo(TipoMensaje::NEW_ROOM)
				     .setRoomname("Sala 1")
				     .build());

  std::string lineaCrea = creaSala.dump() + "\n";
  send(socketCnx1, lineaCrea.data(), lineaCrea.size(), 0);

  bytesLeidos = read(socketCnx1, datos, sizeof(datos));
  std::string linea(datos, bytesLeidos);
  json jsonRespuesta = json::parse(linea);

  json jsonEsperado = GeneraJSON::genera(Mensaje::Builder()
					 .setTipo(TipoMensaje::RESPONSE)
					 .setOperacion(OperacionMensaje::NEW_ROOM)
					 .setResultado(ResultadoMensaje::SUCCESS)
					 .setExtra("Sala 1")
					 .build());

  EXPECT_EQ(jsonEsperado, jsonRespuesta);

  send(socketCnx1, lineaCrea.data(), lineaCrea.size(), 0);

  bytesLeidos = read(socketCnx1, datos, sizeof(datos));
  linea = std::string(datos, bytesLeidos);
  jsonRespuesta = json::parse(linea);

  jsonEsperado = GeneraJSON::genera(Mensaje::Builder()
					 .setTipo(TipoMensaje::RESPONSE)
					 .setOperacion(OperacionMensaje::NEW_ROOM)
					 .setResultado(ResultadoMensaje::ROOM_ALREADY_EXISTS)
					 .setExtra("Sala 1")
					 .build());

  EXPECT_EQ(jsonEsperado, jsonRespuesta);
  
  close(socketCnx1);

  servidor.detenerServidor();
  
  hiloServidor.join();
}

/**
 * @brief Prueba que se puedan invitar usuarios a una sala.
 */
TEST(ServidorTest, InvitaSala){
  std::thread hiloServidor;
  
  int puerto = puertoAleatorio();
  char datos[1024];
  
  Servidor servidor(puerto);

  hiloServidor = std::thread([&](){
    servidor.sirve();
  });

  int socketCnx1 = socket(AF_INET, SOCK_STREAM, 0);
  int socketCnx2 = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in direccion;
  direccion.sin_family = AF_INET;
  direccion.sin_addr.s_addr = inet_addr("127.0.0.1");
  direccion.sin_port = htons(puerto);

  connect(socketCnx1, (sockaddr*)&direccion, sizeof(direccion));
  connect(socketCnx2, (sockaddr*)&direccion, sizeof(direccion));
    
  json jsonAlice = GeneraJSON::genera(Mensaje::Builder()
				      .setTipo(TipoMensaje::IDENTIFY)
				      .setUsername("Alice")
				      .build());
    
  json jsonCharlie = GeneraJSON::genera(Mensaje::Builder()
					.setTipo(TipoMensaje::IDENTIFY)
					.setUsername("Charlie")
					.build());
    
  std::string lineaAlice = jsonAlice.dump() + "\n";
  std::string lineaCharlie = jsonCharlie.dump() + "\n";

  send(socketCnx1, lineaAlice.data(), lineaAlice.size(), 0);

  auto bytesLeidos = read(socketCnx1, datos, sizeof(datos));

  send(socketCnx2, lineaCharlie.data(), lineaCharlie.size(), 0);
    
  bytesLeidos = read(socketCnx2, datos, sizeof(datos));

  bytesLeidos = read(socketCnx1, datos, sizeof(datos));
  
  json creaSala = GeneraJSON::genera(Mensaje::Builder()
				     .setTipo(TipoMensaje::NEW_ROOM)
				     .setRoomname("Sala 1")
				     .build());

  std::string lineaCrea = creaSala.dump() + "\n";
  send(socketCnx1, lineaCrea.data(), lineaCrea.size(), 0);

  bytesLeidos = read(socketCnx1, datos, sizeof(datos));

  json invita = GeneraJSON::genera(Mensaje::Builder()
				   .setTipo(TipoMensaje::INVITE)
				   .setRoomname("Sala 1")
				   .setUsernames({"Charlie"})
				   .build());

  std::string lineaInvita = invita.dump() + "\n";
  send(socketCnx1, lineaInvita.data(), lineaInvita.size(), 0);

  bytesLeidos = read(socketCnx2, datos, sizeof(datos));
  std::string linea(datos, bytesLeidos);
  json jsonRespuesta = json::parse(linea);

  json jsonEsperado = GeneraJSON::genera(Mensaje::Builder()
					 .setTipo(TipoMensaje::INVITATION)
					 .setUsername("Alice")
					 .setRoomname("Sala 1")
					 .build());

  EXPECT_EQ(jsonEsperado, jsonRespuesta);

  invita = GeneraJSON::genera(Mensaje::Builder()
				   .setTipo(TipoMensaje::INVITE)
				   .setRoomname("Sala 2")
				   .setUsernames({"Charlie"})
				   .build());

  lineaInvita = invita.dump() + "\n";
  send(socketCnx1, lineaInvita.data(), lineaInvita.size(), 0);

  bytesLeidos = read(socketCnx1, datos, sizeof(datos));
  linea = std::string(datos, bytesLeidos);
  jsonRespuesta = json::parse(linea);

  jsonEsperado = GeneraJSON::genera(Mensaje::Builder()
				    .setTipo(TipoMensaje::RESPONSE)
				    .setOperacion(OperacionMensaje::INVITE)
				    .setResultado(ResultadoMensaje::NO_SUCH_ROOM)
				    .setExtra("Sala 2")
				    .build());

  EXPECT_EQ(jsonEsperado, jsonRespuesta);

  invita = GeneraJSON::genera(Mensaje::Builder()
			      .setTipo(TipoMensaje::INVITE)
			      .setRoomname("Sala 1")
			      .setUsernames({"Bob"})
			      .build());

  lineaInvita = invita.dump() + "\n";
  send(socketCnx1, lineaInvita.data(), lineaInvita.size(), 0);
  
  bytesLeidos = read(socketCnx1, datos, sizeof(datos));
  linea = std::string(datos, bytesLeidos);
  jsonRespuesta = json::parse(linea);

  jsonEsperado = GeneraJSON::genera(Mensaje::Builder()
				    .setTipo(TipoMensaje::RESPONSE)
				    .setOperacion(OperacionMensaje::INVITE)
				    .setResultado(ResultadoMensaje::NO_SUCH_USER)
				    .setExtra("Bob")
				    .build());

  EXPECT_EQ(jsonEsperado, jsonRespuesta);
    
  close(socketCnx1);
  close(socketCnx2);

  servidor.detenerServidor();
  
  hiloServidor.join();
}

/**
 * @brief Prueba que las conexiones se puedan unir a una sala.
 */
TEST(ServidorTest, EntrarSala){
  std::thread hiloServidor;
  
  int puerto = puertoAleatorio();
  char datos[1024];
  
  Servidor servidor(puerto);

  hiloServidor = std::thread([&](){
    servidor.sirve();
  });

  int socketCnx1 = socket(AF_INET, SOCK_STREAM, 0);
  int socketCnx2 = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in direccion;
  direccion.sin_family = AF_INET;
  direccion.sin_addr.s_addr = inet_addr("127.0.0.1");
  direccion.sin_port = htons(puerto);

  connect(socketCnx1, (sockaddr*)&direccion, sizeof(direccion));
  connect(socketCnx2, (sockaddr*)&direccion, sizeof(direccion));
    
  json jsonAlice = GeneraJSON::genera(Mensaje::Builder()
				      .setTipo(TipoMensaje::IDENTIFY)
				      .setUsername("Alice")
				      .build());
    
  json jsonCharlie = GeneraJSON::genera(Mensaje::Builder()
					.setTipo(TipoMensaje::IDENTIFY)
					.setUsername("Charlie")
					.build());
    
  std::string lineaAlice = jsonAlice.dump() + "\n";
  std::string lineaCharlie = jsonCharlie.dump() + "\n";

  send(socketCnx1, lineaAlice.data(), lineaAlice.size(), 0);

  auto bytesLeidos = read(socketCnx1, datos, sizeof(datos));

  send(socketCnx2, lineaCharlie.data(), lineaCharlie.size(), 0);
    
  bytesLeidos = read(socketCnx2, datos, sizeof(datos));

  bytesLeidos = read(socketCnx1, datos, sizeof(datos));

  json entraSala = GeneraJSON::genera(Mensaje::Builder()
				      .setTipo(TipoMensaje::JOIN_ROOM)
				      .setRoomname("Sala 1")
				      .build());

  std::string lineaEntra = entraSala.dump() + "\n";
  send(socketCnx2, lineaEntra.data(), lineaEntra.size(), 0);

  bytesLeidos = read(socketCnx2, datos, sizeof(datos));
  std::string linea(datos, bytesLeidos);
  json jsonRespuesta = json::parse(linea);
  
  json jsonEsperado = GeneraJSON::genera(Mensaje::Builder()
					 .setTipo(TipoMensaje::RESPONSE)
					 .setOperacion(OperacionMensaje::JOIN_ROOM)
					 .setResultado(ResultadoMensaje::NO_SUCH_ROOM)
					 .setExtra("Sala 1")
					 .build());

  EXPECT_EQ(jsonEsperado, jsonRespuesta);
  
  json creaSala = GeneraJSON::genera(Mensaje::Builder()
				     .setTipo(TipoMensaje::NEW_ROOM)
				     .setRoomname("Sala 1")
				     .build());

  std::string lineaCrea = creaSala.dump() + "\n";
  send(socketCnx1, lineaCrea.data(), lineaCrea.size(), 0);

  bytesLeidos = read(socketCnx1, datos, sizeof(datos));

  send(socketCnx2, lineaEntra.data(), lineaEntra.size(), 0);

  bytesLeidos = read(socketCnx2, datos, sizeof(datos));
  linea = std::string(datos, bytesLeidos);
  jsonRespuesta = json::parse(linea);
  
  jsonEsperado = GeneraJSON::genera(Mensaje::Builder()
				    .setTipo(TipoMensaje::RESPONSE)
				    .setOperacion(OperacionMensaje::JOIN_ROOM)
				    .setResultado(ResultadoMensaje::NOT_INVITED)
				    .setExtra("Sala 1")
				    .build());

  EXPECT_EQ(jsonEsperado, jsonRespuesta);

  json invita = GeneraJSON::genera(Mensaje::Builder()
				   .setTipo(TipoMensaje::INVITE)
				   .setRoomname("Sala 1")
				   .setUsernames({"Charlie"})
				   .build());

  std::string lineaInvita = invita.dump() + "\n";
  send(socketCnx1, lineaInvita.data(), lineaInvita.size(), 0);

  bytesLeidos = read(socketCnx2, datos, sizeof(datos));

  send(socketCnx2, lineaEntra.data(), lineaEntra.size(), 0);

  bytesLeidos = read(socketCnx2, datos, sizeof(datos));
  linea = std::string(datos, bytesLeidos);
  jsonRespuesta = json::parse(linea);
  
  jsonEsperado = GeneraJSON::genera(Mensaje::Builder()
				    .setTipo(TipoMensaje::RESPONSE)
				    .setOperacion(OperacionMensaje::JOIN_ROOM)
				    .setResultado(ResultadoMensaje::SUCCESS)
				    .setExtra("Sala 1")
				    .build());

  EXPECT_EQ(jsonEsperado, jsonRespuesta);

  bytesLeidos = read(socketCnx1, datos, sizeof(datos));
  linea = std::string(datos, bytesLeidos);
  jsonRespuesta = json::parse(linea);

  jsonEsperado = GeneraJSON::genera(Mensaje::Builder()
				    .setTipo(TipoMensaje::JOINED_ROOM)
				    .setRoomname("Sala 1")
				    .setUsername("Charlie")
				    .build());

  EXPECT_EQ(jsonEsperado, jsonRespuesta);

  close(socketCnx1);
  close(socketCnx2);

  servidor.detenerServidor();
  
  hiloServidor.join();
}

/**
 * @brief Prueba que se envie correctame la lista de usuarios en la sala.
 */
TEST(ServidorTest, ListaSalaUsuarios){
  std::thread hiloServidor;
  
  int puerto = puertoAleatorio();
  char datos[1024];
  
  Servidor servidor(puerto);

  hiloServidor = std::thread([&](){
    servidor.sirve();
  });

  int socketCnx1 = socket(AF_INET, SOCK_STREAM, 0);
  int socketCnx2 = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in direccion;
  direccion.sin_family = AF_INET;
  direccion.sin_addr.s_addr = inet_addr("127.0.0.1");
  direccion.sin_port = htons(puerto);

  connect(socketCnx1, (sockaddr*)&direccion, sizeof(direccion));
  connect(socketCnx2, (sockaddr*)&direccion, sizeof(direccion));
    
  json jsonAlice = GeneraJSON::genera(Mensaje::Builder()
				      .setTipo(TipoMensaje::IDENTIFY)
				      .setUsername("Alice")
				      .build());
  
  json jsonCharlie = GeneraJSON::genera(Mensaje::Builder()
					.setTipo(TipoMensaje::IDENTIFY)
					.setUsername("Charlie")
					.build());
    
  std::string lineaAlice = jsonAlice.dump() + "\n";
  std::string lineaCharlie = jsonCharlie.dump() + "\n";

  send(socketCnx1, lineaAlice.data(), lineaAlice.size(), 0);
  auto bytesLeidos = read(socketCnx1, datos, sizeof(datos));

  send(socketCnx2, lineaCharlie.data(), lineaCharlie.size(), 0);
  bytesLeidos = read(socketCnx2, datos, sizeof(datos));
  read(socketCnx1, datos, sizeof(datos));

  json listaSala = GeneraJSON::genera(Mensaje::Builder()
					 .setTipo(TipoMensaje::ROOM_USERS)
					 .setRoomname("Sala 1")
					 .build());

  std::string lineaLista = listaSala.dump() + "\n";
  send(socketCnx1, lineaLista.data(), lineaLista.size(), 0);
  bytesLeidos = read(socketCnx1, datos, sizeof(datos));
  std::string linea(datos, bytesLeidos);
  json jsonRespuesta = json::parse(linea);
  
  json jsonEsperado = GeneraJSON::genera(Mensaje::Builder()
					 .setTipo(TipoMensaje::RESPONSE)
					 .setOperacion(OperacionMensaje::ROOM_USERS)
					 .setResultado(ResultadoMensaje::NO_SUCH_ROOM)
					 .setExtra("Sala 1")
					 .build());

  EXPECT_EQ(jsonRespuesta, jsonEsperado);

  json creaSala = GeneraJSON::genera(Mensaje::Builder()
				     .setTipo(TipoMensaje::NEW_ROOM)
				     .setRoomname("Sala 1")
				     .build());

  std::string lineaCrea = creaSala.dump() + "\n";
  send(socketCnx1, lineaCrea.data(), lineaCrea.size(), 0);

  bytesLeidos = read(socketCnx1, datos, sizeof(datos));

  send(socketCnx1, lineaLista.data(), lineaLista.size(), 0);
  bytesLeidos = read(socketCnx1, datos, sizeof(datos));
  linea = std::string(datos, bytesLeidos);
  jsonRespuesta = json::parse(linea);

  std::unordered_map<std::string, EstatusUsuario> usuarios = {{"Alice", EstatusUsuario::ACTIVE}};
  
  jsonEsperado = GeneraJSON::genera(Mensaje::Builder()
				    .setTipo(TipoMensaje::ROOM_USER_LIST)
				    .setRoomname("Sala 1")
				    .setUsers(usuarios)
				    .build());
  
  EXPECT_EQ(jsonRespuesta, jsonEsperado);

  send(socketCnx2, lineaLista.data(), lineaLista.size(), 0);
  bytesLeidos = read(socketCnx2, datos, sizeof(datos));
  linea = std::string(datos, bytesLeidos);
  jsonRespuesta = json::parse(linea);

  jsonEsperado = GeneraJSON::genera(Mensaje::Builder()
				    .setTipo(TipoMensaje::RESPONSE)
				    .setOperacion(OperacionMensaje::ROOM_USERS)
				    .setResultado(ResultadoMensaje::NOT_JOINED)
				    .setExtra("Sala 1")
				    .build());

  EXPECT_EQ(jsonRespuesta, jsonEsperado);
  
  close(socketCnx1);
  close(socketCnx2);

  servidor.detenerServidor();
  
  hiloServidor.join();
}

/**
 * @brief Prueba que se envie correctamente un texto a una sala.
 */
TEST(ServidorTest, EnviaTextoSala){
  std::thread hiloServidor;
  
  int puerto = puertoAleatorio();
  char datos[1024];
  
  Servidor servidor(puerto);

  hiloServidor = std::thread([&](){
    servidor.sirve();
  });

  int socketCnx1 = socket(AF_INET, SOCK_STREAM, 0);
  int socketCnx2 = socket(AF_INET, SOCK_STREAM, 0);
  int socketCnx3 = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in direccion;
  direccion.sin_family = AF_INET;
  direccion.sin_addr.s_addr = inet_addr("127.0.0.1");
  direccion.sin_port = htons(puerto);

  connect(socketCnx1, (sockaddr*)&direccion, sizeof(direccion));
  connect(socketCnx2, (sockaddr*)&direccion, sizeof(direccion));
  connect(socketCnx3, (sockaddr*)&direccion, sizeof(direccion));
    
  json jsonAlice = GeneraJSON::genera(Mensaje::Builder()
				      .setTipo(TipoMensaje::IDENTIFY)
				      .setUsername("Alice")
				      .build());
  
  json jsonCharlie = GeneraJSON::genera(Mensaje::Builder()
					.setTipo(TipoMensaje::IDENTIFY)
					.setUsername("Charlie")
					.build());

  json jsonBob = GeneraJSON::genera(Mensaje::Builder()
					.setTipo(TipoMensaje::IDENTIFY)
					.setUsername("Bob")
					.build());
    
  std::string lineaAlice = jsonAlice.dump() + "\n";
  std::string lineaCharlie = jsonCharlie.dump() + "\n";
  std::string lineaBob = jsonBob.dump() + "\n";

  send(socketCnx1, lineaAlice.data(), lineaAlice.size(), 0);
  auto bytesLeidos = read(socketCnx1, datos, sizeof(datos));

  send(socketCnx2, lineaCharlie.data(), lineaCharlie.size(), 0);
  bytesLeidos = read(socketCnx2, datos, sizeof(datos));
  read(socketCnx1, datos, sizeof(datos));

  send(socketCnx3, lineaBob.data(), lineaBob.size(), 0);
  bytesLeidos = read(socketCnx3, datos, sizeof(datos));
  read(socketCnx1, datos, sizeof(datos));
  read(socketCnx2, datos, sizeof(datos));

  json creaSala = GeneraJSON::genera(Mensaje::Builder()
				     .setTipo(TipoMensaje::NEW_ROOM)
				     .setRoomname("Sala 1")
				     .build());

  std::string lineaCrea = creaSala.dump() + "\n";
  send(socketCnx1, lineaCrea.data(), lineaCrea.size(), 0);

  bytesLeidos = read(socketCnx1, datos, sizeof(datos));

  json invita = GeneraJSON::genera(Mensaje::Builder()
				   .setTipo(TipoMensaje::INVITE)
				   .setRoomname("Sala 1")
				   .setUsernames({"Charlie"})
				   .build());

  std::string lineaInvita = invita.dump() + "\n";
  send(socketCnx1, lineaInvita.data(), lineaInvita.size(), 0);
  
  bytesLeidos = read(socketCnx2, datos, sizeof(datos));
  
  json entraSala = GeneraJSON::genera(Mensaje::Builder()
				      .setTipo(TipoMensaje::JOIN_ROOM)
				      .setRoomname("Sala 1")
				      .build());

  std::string lineaEntra = entraSala.dump() + "\n";
  send(socketCnx2, lineaEntra.data(), lineaEntra.size(), 0);

  read(socketCnx1, datos, sizeof(datos));
  bytesLeidos = read(socketCnx2, datos, sizeof(datos));

  json enviaTexto = GeneraJSON::genera(Mensaje::Builder()
				       .setTipo(TipoMensaje::ROOM_TEXT)
				       .setRoomname("Sala 1")
				       .setText("Hola Sala")
				       .build());

  std::string lineaTexto = enviaTexto.dump() + "\n";
  send(socketCnx1, lineaTexto.data(), lineaTexto.size(), 0);
  
  bytesLeidos = read(socketCnx2, datos, sizeof(datos));
  std::string linea(datos, bytesLeidos);
  json jsonRespuesta = json::parse(linea);

  json jsonEsperado = GeneraJSON::genera(Mensaje::Builder()
					 .setTipo(TipoMensaje::ROOM_TEXT_FROM)
					 .setRoomname("Sala 1")
					 .setUsername("Alice")
					 .setText("Hola Sala")
					 .build());

  EXPECT_EQ(jsonEsperado, jsonRespuesta);
  
  enviaTexto = GeneraJSON::genera(Mensaje::Builder()
				       .setTipo(TipoMensaje::ROOM_TEXT)
				       .setRoomname("Sala 2")
				       .setText("Hola Sala")
				       .build());

  lineaTexto = enviaTexto.dump() + "\n";
  send(socketCnx1, lineaTexto.data(), lineaTexto.size(), 0);

  bytesLeidos = read(socketCnx1, datos, sizeof(datos));
  linea = std::string(datos, bytesLeidos);
  jsonRespuesta = json::parse(linea);

  jsonEsperado = GeneraJSON::genera(Mensaje::Builder()
				    .setTipo(TipoMensaje::RESPONSE)
				    .setOperacion(OperacionMensaje::ROOM_TEXT)
				    .setResultado(ResultadoMensaje::NO_SUCH_ROOM)
				    .setExtra("Sala 2")
				    .build());
  
  EXPECT_EQ(jsonEsperado, jsonRespuesta);
  
  enviaTexto = GeneraJSON::genera(Mensaje::Builder()
				  .setTipo(TipoMensaje::ROOM_TEXT)
				  .setRoomname("Sala 1")
				  .setText("Hola Sala")
				  .build());
  
  lineaTexto = enviaTexto.dump() + "\n";
  send(socketCnx3, lineaTexto.data(), lineaTexto.size(), 0);
  
  bytesLeidos = read(socketCnx3, datos, sizeof(datos));
  linea = std::string(datos, bytesLeidos);
  jsonRespuesta = json::parse(linea);

  jsonEsperado = GeneraJSON::genera(Mensaje::Builder()
				    .setTipo(TipoMensaje::RESPONSE)
				    .setOperacion(OperacionMensaje::ROOM_TEXT)
				    .setResultado(ResultadoMensaje::NOT_JOINED)
				    .setExtra("Sala 1")
				    .build());
  
  EXPECT_EQ(jsonEsperado, jsonRespuesta);
  
  close(socketCnx1);
  close(socketCnx2);
  close(socketCnx3);

  servidor.detenerServidor();
  
  hiloServidor.join();
}

/**
 * @brief Prueba que una conexión se pueda salir de una sala.
 */
TEST(ServidorTest, SalirSala){
  std::thread hiloServidor;
  
  int puerto = puertoAleatorio();
  char datos[1024];
  
  Servidor servidor(puerto);

  hiloServidor = std::thread([&](){
    servidor.sirve();
  });

  int socketCnx1 = socket(AF_INET, SOCK_STREAM, 0);
  int socketCnx2 = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in direccion;
  direccion.sin_family = AF_INET;
  direccion.sin_addr.s_addr = inet_addr("127.0.0.1");
  direccion.sin_port = htons(puerto);

  connect(socketCnx1, (sockaddr*)&direccion, sizeof(direccion));
  connect(socketCnx2, (sockaddr*)&direccion, sizeof(direccion));
    
  json jsonAlice = GeneraJSON::genera(Mensaje::Builder()
				      .setTipo(TipoMensaje::IDENTIFY)
				      .setUsername("Alice")
				      .build());
    
  json jsonCharlie = GeneraJSON::genera(Mensaje::Builder()
					.setTipo(TipoMensaje::IDENTIFY)
					.setUsername("Charlie")
					.build());
    
  std::string lineaAlice = jsonAlice.dump() + "\n";
  std::string lineaCharlie = jsonCharlie.dump() + "\n";

  send(socketCnx1, lineaAlice.data(), lineaAlice.size(), 0);

  auto bytesLeidos = read(socketCnx1, datos, sizeof(datos));

  send(socketCnx2, lineaCharlie.data(), lineaCharlie.size(), 0);

  bytesLeidos = read(socketCnx1, datos, sizeof(datos));
  read(socketCnx2, datos, sizeof(datos));
  
  json creaSala = GeneraJSON::genera(Mensaje::Builder()
				     .setTipo(TipoMensaje::NEW_ROOM)
				     .setRoomname("Sala 1")
				     .build());

  std::string lineaCrea = creaSala.dump() + "\n";
  send(socketCnx1, lineaCrea.data(), lineaCrea.size(), 0);

  bytesLeidos = read(socketCnx1, datos, sizeof(datos));

  json invita = GeneraJSON::genera(Mensaje::Builder()
				   .setTipo(TipoMensaje::INVITE)
				   .setRoomname("Sala 1")
				   .setUsernames({"Charlie"})
				   .build());

  std::string lineaInvita = invita.dump() + "\n";
  send(socketCnx1, lineaInvita.data(), lineaInvita.size(), 0);

  bytesLeidos = read(socketCnx2, datos, sizeof(datos));

  json entraSala = GeneraJSON::genera(Mensaje::Builder()
				      .setTipo(TipoMensaje::JOIN_ROOM)
				      .setRoomname("Sala 1")
				      .build());
  
  std::string lineaEntra = entraSala.dump() + "\n";
  send(socketCnx2, lineaEntra.data(), lineaEntra.size(), 0);

  bytesLeidos = read(socketCnx2, datos, sizeof(datos));

  bytesLeidos = read(socketCnx1, datos, sizeof(datos));

  json saleSala = GeneraJSON::genera(Mensaje::Builder()
				     .setTipo(TipoMensaje::LEAVE_ROOM)
				     .setRoomname("Sala 1")
				     .build());

  std::string lineaSale = saleSala.dump() + "\n";
  send(socketCnx2, lineaSale.data(), lineaSale.size(), 0);

  bytesLeidos = read(socketCnx1, datos, sizeof(datos));
  std::string linea(datos, bytesLeidos);
  json jsonRespuesta = json::parse(linea);

  json jsonEsperado = GeneraJSON::genera(Mensaje::Builder()
					 .setTipo(TipoMensaje::LEFT_ROOM)
					 .setRoomname("Sala 1")
					 .setUsername("Charlie")
					 .build());

  EXPECT_EQ(jsonEsperado, jsonRespuesta);

  saleSala = GeneraJSON::genera(Mensaje::Builder()
				     .setTipo(TipoMensaje::LEAVE_ROOM)
				     .setRoomname("Sala 2")
				     .build());

  lineaSale = saleSala.dump() + "\n";
  send(socketCnx1, lineaSale.data(), lineaSale.size(), 0);
  bytesLeidos = read(socketCnx1, datos, sizeof(datos));
  linea = std::string(datos, bytesLeidos);
  jsonRespuesta = json::parse(linea);

  jsonEsperado = GeneraJSON::genera(Mensaje::Builder()
				    .setTipo(TipoMensaje::RESPONSE)
				    .setOperacion(OperacionMensaje::LEAVE_ROOM)
				    .setResultado(ResultadoMensaje::NO_SUCH_ROOM)
				    .setExtra("Sala 2")
				    .build());

  EXPECT_EQ(jsonEsperado, jsonRespuesta);

  saleSala = GeneraJSON::genera(Mensaje::Builder()
				.setTipo(TipoMensaje::LEAVE_ROOM)
				.setRoomname("Sala 1")
				.build());
  
  lineaSale = saleSala.dump() + "\n";
  send(socketCnx2, lineaSale.data(), lineaSale.size(), 0);
  bytesLeidos = read(socketCnx2, datos, sizeof(datos));
  linea = std::string(datos, bytesLeidos);
  jsonRespuesta = json::parse(linea);

  jsonEsperado = GeneraJSON::genera(Mensaje::Builder()
				    .setTipo(TipoMensaje::RESPONSE)
				    .setOperacion(OperacionMensaje::LEAVE_ROOM)
				    .setResultado(ResultadoMensaje::NOT_JOINED)
				    .setExtra("Sala 1")
				    .build());

  EXPECT_EQ(jsonEsperado, jsonRespuesta);

  close(socketCnx1);
  close(socketCnx2);

  servidor.detenerServidor();
  
  hiloServidor.join();
}

/**
 * @brief Prueba que una conexión se pueda desconectar.
 */
TEST(ServidorTest, Desconecta){
  std::thread hiloServidor;
  
  int puerto = puertoAleatorio();
  char datos[1024];
  
  Servidor servidor(puerto);

  hiloServidor = std::thread([&](){
    servidor.sirve();
  });

  int socketCnx1 = socket(AF_INET, SOCK_STREAM, 0);
  int socketCnx2 = socket(AF_INET, SOCK_STREAM, 0);
  int socketCnx3 = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in direccion;
  direccion.sin_family = AF_INET;
  direccion.sin_addr.s_addr = inet_addr("127.0.0.1");
  direccion.sin_port = htons(puerto);

  connect(socketCnx1, (sockaddr*)&direccion, sizeof(direccion));
  connect(socketCnx2, (sockaddr*)&direccion, sizeof(direccion));
  connect(socketCnx3, (sockaddr*)&direccion, sizeof(direccion));
    
  json jsonAlice = GeneraJSON::genera(Mensaje::Builder()
				      .setTipo(TipoMensaje::IDENTIFY)
				      .setUsername("Alice")
				      .build());
  
  json jsonCharlie = GeneraJSON::genera(Mensaje::Builder()
					.setTipo(TipoMensaje::IDENTIFY)
					.setUsername("Charlie")
					.build());

  json jsonBob = GeneraJSON::genera(Mensaje::Builder()
					.setTipo(TipoMensaje::IDENTIFY)
					.setUsername("Bob")
					.build());
    
  std::string lineaAlice = jsonAlice.dump() + "\n";
  std::string lineaCharlie = jsonCharlie.dump() + "\n";
  std::string lineaBob = jsonBob.dump() + "\n";

  send(socketCnx1, lineaAlice.data(), lineaAlice.size(), 0);
  auto bytesLeidos = read(socketCnx1, datos, sizeof(datos));

  send(socketCnx2, lineaCharlie.data(), lineaCharlie.size(), 0);
  bytesLeidos = read(socketCnx2, datos, sizeof(datos));
  read(socketCnx1, datos, sizeof(datos));

  send(socketCnx3, lineaBob.data(), lineaBob.size(), 0);
  bytesLeidos = read(socketCnx3, datos, sizeof(datos));
  read(socketCnx1, datos, sizeof(datos));
  read(socketCnx2, datos, sizeof(datos));


  json creaSala = GeneraJSON::genera(Mensaje::Builder()
				     .setTipo(TipoMensaje::NEW_ROOM)
				     .setRoomname("Sala 1")
				     .build());

  std::string lineaCrea = creaSala.dump() + "\n";
  send(socketCnx1, lineaCrea.data(), lineaCrea.size(), 0);

  bytesLeidos = read(socketCnx1, datos, sizeof(datos));

  json invita = GeneraJSON::genera(Mensaje::Builder()
				   .setTipo(TipoMensaje::INVITE)
				   .setRoomname("Sala 1")
				   .setUsernames({"Charlie"})
				   .build());

  std::string lineaInvita = invita.dump() + "\n";
  send(socketCnx1, lineaInvita.data(), lineaInvita.size(), 0);

  bytesLeidos = read(socketCnx2, datos, sizeof(datos));

  json entraSala = GeneraJSON::genera(Mensaje::Builder()
				      .setTipo(TipoMensaje::JOIN_ROOM)
				      .setRoomname("Sala 1")
				      .build());
  
  std::string lineaEntra = entraSala.dump() + "\n";
  send(socketCnx2, lineaEntra.data(), lineaEntra.size(), 0);

  bytesLeidos = read(socketCnx2, datos, sizeof(datos));

  bytesLeidos = read(socketCnx1, datos, sizeof(datos));

  json desconecta = GeneraJSON::genera(Mensaje::Builder()
				       .setTipo(TipoMensaje::DISCONNECT)
				       .build());

  std::string lineaDesconecta = desconecta.dump() + "\n";
  send(socketCnx2, lineaDesconecta.data(), lineaDesconecta.size(), 0);

  bytesLeidos = read(socketCnx1, datos, sizeof(datos));
  std::string linea(datos, bytesLeidos);
  int posicion = linea.find('\n');
  
  std::string leftMensaje = linea.substr(0, posicion);
  linea.erase(0, posicion + 1);

  json jsonRespuesta = json::parse(leftMensaje);

  json jsonEsperado = GeneraJSON::genera(Mensaje::Builder()
				    .setTipo(TipoMensaje::LEFT_ROOM)
				    .setRoomname("Sala 1")
				    .setUsername("Charlie")
				    .build());

  EXPECT_EQ(jsonEsperado, jsonRespuesta);

  
  if(linea.empty()){
    bytesLeidos = read(socketCnx1, datos, sizeof(datos));
    linea = std::string(datos, bytesLeidos);
  }
  
  posicion = linea.find('\n');
  std::string disconnectedMensaje = linea.substr(0, posicion);
  
  jsonRespuesta = json::parse(disconnectedMensaje);
  
  jsonEsperado = GeneraJSON::genera(Mensaje::Builder()
				    .setTipo(TipoMensaje::DISCONNECTED)
				    .setUsername("Charlie")
				    .build());
  
  EXPECT_EQ(jsonEsperado, jsonRespuesta);

  bytesLeidos = read(socketCnx3, datos, sizeof(datos));
  linea = std::string(datos, bytesLeidos);
  jsonRespuesta = json::parse(linea);
  
  EXPECT_EQ(jsonEsperado, jsonRespuesta);

  close(socketCnx1);
  close(socketCnx2);
  close(socketCnx3);

  servidor.detenerServidor();
  
  hiloServidor.join();
}
