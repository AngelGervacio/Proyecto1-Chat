/**
 * @file TestMensaje.cpp
 * @brief Pruebas unitarias para la clase Mensaje.
 */

#include <gtest/gtest.h>
#include <string>
#include <list>
#include <random>
#include <vector>
#include "Usuario.hpp"
#include "EstatusUsuario.hpp"
#include "Mensaje.hpp"
#include "TipoMensaje.hpp"
#include "OperacionMensaje.hpp"
#include "ResultadoMensaje.hpp"

/**
 * @brief Regresa un tipo de Mensaje aleatorio.
 * @return un tipo de Mensaje aleatorio.
 */
TipoMensaje tipoAleatorio(){
  const std::vector<TipoMensaje> tipos = {
    TipoMensaje::IDENTIFY,
    TipoMensaje::RESPONSE,
    TipoMensaje::NEW_USER,
    TipoMensaje::STATUS,
    TipoMensaje::NEW_STATUS,
    TipoMensaje::USERS,
    TipoMensaje::USER_LIST,
    TipoMensaje::TEXT,
    TipoMensaje::TEXT_FROM,
    TipoMensaje::PUBLIC_TEXT,
    TipoMensaje::PUBLIC_TEXT_FROM,
    TipoMensaje::NEW_ROOM,
    TipoMensaje::INVITE,
    TipoMensaje::INVITATION,
    TipoMensaje::JOIN_ROOM,
    TipoMensaje::JOINED_ROOM,
    TipoMensaje::ROOM_USERS,
    TipoMensaje::ROOM_USERS_LIST,
    TipoMensaje::ROOM_TEXT,
    TipoMensaje::ROOM_TEXT_FROM,
    TipoMensaje::LEAVE_ROOM,
    TipoMensaje::LEFT_ROOM,
    TipoMensaje::DISCONNECT,
    TipoMensaje::DISCONNECTED
  };
  
  std::random_device random;
  std::mt19937 gen(random());
  std::uniform_int_distribution<> distrib(0, tipos.size() - 1);

  int indice = distrib(gen);

  return tipos[indice];
}

/**
 * @brief Regresa una operación del Mensaje aleatoria.
 * @return una operación del Mensaje aleatoria.
 */
OperacionMensaje operacionAleatoria(){
  const std::vector<OperacionMensaje> operaciones = {
    OperacionMensaje::IDENTIFY,
    OperacionMensaje::TEXT,
    OperacionMensaje::NEW_ROOM,
    OperacionMensaje::INVITE,
    OperacionMensaje::JOIN_ROOM,
    OperacionMensaje::ROOM_USERS,
    OperacionMensaje::ROOM_TEXT,
    OperacionMensaje::LEAVE_ROOM,
    OperacionMensaje::INVALID
  };
  
  std::random_device random;
  std::mt19937 gen(random());
  std::uniform_int_distribution<> distrib(0, operaciones.size() - 1);

  int indice = distrib(gen);

  return operaciones[indice];
}

/**
 * @brief Regresa un resultado aleatorio.
 * @return un resultado aleatorio.
 */
ResultadoMensaje resultadoAleatorio(){
  const std::vector<ResultadoMensaje> resultados = {
    ResultadoMensaje::SUCCESS,
    ResultadoMensaje::USER_ALREADY_EXISTS,
    ResultadoMensaje::NO_SUCH_USER,
    ResultadoMensaje::ROOM_ALREADY_EXISTS,
    ResultadoMensaje::NO_SUCH_ROOM,
    ResultadoMensaje::NOT_INVITED,
    ResultadoMensaje::NOT_JOINED,
    ResultadoMensaje::NOT_IDENTIFIED,
    ResultadoMensaje::INVALID
  };
  
  std::random_device random;
  std::mt19937 gen(random());
  std::uniform_int_distribution<> distrib(0, resultados.size() - 1);

  int indice = distrib(gen);

  return resultados[indice];
}

/**
 * @brief Regresa un estatus de Usuario aleatorio.
 * @return un estatus de Usuario aleatorio.
 */
EstatusUsuario estatusAleatorio(){
  const std::vector<EstatusUsuario> estatus = {
    EstatusUsuario::ACTIVE,
    EstatusUsuario::AWAY,
    EstatusUsuario::BUSY
  };
  
  std::random_device random;
  std::mt19937 gen(random());
  std::uniform_int_distribution<> distrib(0, estatus.size() - 1);

  int indice = distrib(gen);

  return estatus[indice];
}

/**
 * @brief Regresa un nombre aleatorio,
 * @return un nombre aleatorio.
 */
std::string nombreAleatorio(){
  const std::vector<std::string> nombres = {
    "Alice", "Bob", "Charlie", "Daniel"
  };
  
  std::random_device random;
  std::mt19937 gen(random());
  std::uniform_int_distribution<> distrib(0, nombres.size() - 1);

  int indice = distrib(gen);

  return nombres[indice];
}

/**
 * @brief Prueba los metodos setTipo() y getTipo() de la clase Mensaje.
 * Comprueba que se asigne correctamente el tipo.
 */
TEST(MensajeTest, AgregaObtieneTipo){
  for(int i = 0; i < 20; i++){
    TipoMensaje tipo = tipoAleatorio();
  
    Mensaje mensaje = Mensaje::Builder()
      .setTipo(tipo)
      .build();

    EXPECT_EQ(mensaje.getTipo(), tipo);
  }
}

/**
 * @brief Prueba los metodos setOperacion() y getOperacion() de la clase Mensaje.
 * Comprueba que se asigne correctamente la operación.
 */
TEST(MensajeTest, AgregaObtieneOperacion){
  for(int i = 0; i < 10; i++){
    OperacionMensaje operacion = operacionAleatoria();
  
    Mensaje mensaje = Mensaje::Builder()
      .setTipo(tipoAleatorio())
      .setOperacion(operacion)
      .build();

    EXPECT_TRUE(mensaje.getOperacion().has_value());
    EXPECT_EQ(mensaje.getOperacion().value(), operacion);
  }
}

/**
 * @brief Prueba los metodos setResultado() y getResultado() de la clase Mensaje.
 * Comprueba que se asigne correctamente el resultado.
 */
TEST(MensajeTest, AgregaObtieneResultado){
  for(int i = 0; i < 10; i++){
    ResultadoMensaje resultado = resultadoAleatorio();
  
    Mensaje mensaje = Mensaje::Builder()
      .setTipo(tipoAleatorio())
      .setResultado(resultado)
      .build();

    EXPECT_TRUE(mensaje.getResultado().has_value());
    EXPECT_EQ(mensaje.getResultado().value(), resultado);
  }
}

/**
 * @brief Prueba los metodos setUsername() y getUsername() de la clase Mensaje.
 * Comprueba que se asigne correctamente el username.
 */
TEST(MensajeTest, AgregaObtieneUsername){
  std::string username = nombreAleatorio();
  Mensaje mensaje = Mensaje::Builder()
    .setTipo(tipoAleatorio())
    .setUsername(username)
    .build();

  EXPECT_TRUE(mensaje.getUsername().has_value());
  EXPECT_EQ(mensaje.getUsername().value(), username);

  username = nombreAleatorio();
  
  mensaje = Mensaje::Builder()
    .setTipo(tipoAleatorio())
    .setUsername(username)
    .build();

  EXPECT_TRUE(mensaje.getUsername().has_value());
  EXPECT_EQ(mensaje.getUsername().value(), username);
}

/**
 * @brief Prueba los metodos setExtra() y getExtra() de la clase Mensaje.
 * Comprueba que se asigne correctamente el contenido extra.
 */
TEST(MensajeTest, AgregaObtieneExtra){
  Mensaje mensaje = Mensaje::Builder()
    .setTipo(tipoAleatorio())
    .setExtra("Contenido extra")
    .build();

  EXPECT_TRUE(mensaje.getExtra().has_value());
  EXPECT_EQ(mensaje.getExtra().value(), "Contenido extra");
}

/**
 * @brief Prueba los metodos setEstatus() y getEstatus() de la clase Mensaje.
 * Comprueba que se asigne correctamente el estatus del Usuario.
 */
TEST(MensajeTest, AgregaObtieneEstatus){
  for(int i = 0; i < 6; i++){
    EstatusUsuario estatus = estatusAleatorio();
  
    Mensaje mensaje = Mensaje::Builder()
      .setTipo(tipoAleatorio())
      .setEstatus(estatus)
      .build();

    EXPECT_TRUE(mensaje.getEstatus().has_value());
    EXPECT_EQ(mensaje.getEstatus().value(), estatus);
  }
}

/**
 * @brief Prueba los metodos setUsers() y getUsers() de la clase Mensaje.
 * Comprueba que se asigne correctamente la lista de Usuarios.
 */
TEST(MensajeTest, AgregaObtieneListaUsuarios){
  std::list<Usuario> users;
  for(int i = 0; i < 10; i++){
    users.push_back(Usuario(nombreAleatorio()));
  }
  
  Mensaje mensaje = Mensaje::Builder()
    .setTipo(tipoAleatorio())
    .setUsers(users)
    .build();

  EXPECT_TRUE(mensaje.getUsers().has_value());
  EXPECT_EQ(mensaje.getUsers().value(), users);
}

/**
 * @brief Prueba los metodos setText() y getText() de la clase Mensaje.
 * Comprueba que se asigne correctamente el texto enviado.
 */
TEST(MensajeTest, AgregaObtieneText){
  Mensaje mensaje = Mensaje::Builder()
    .setTipo(tipoAleatorio())
    .setText("¡Hola Mundo!")
    .build();

  EXPECT_TRUE(mensaje.getText().has_value());
  EXPECT_EQ(mensaje.getText().value(), "¡Hola Mundo!");
}

/**
 * @brief Prueba los metodos setRoomname() y getRoomname() de la clase Mensaje.
 * Comprueba que se asigne correctamente el nombre de la sala.
 */
TEST(MensajeTest, AgregaObtieneRoomname){
  Mensaje mensaje = Mensaje::Builder()
    .setTipo(tipoAleatorio())
    .setRoomname("Sala 1")
    .build();

  EXPECT_TRUE(mensaje.getRoomname().has_value());
  EXPECT_EQ(mensaje.getRoomname().value(), "Sala 1");
  
  mensaje = Mensaje::Builder()
    .setTipo(tipoAleatorio())
    .setRoomname("Sala nueva")
    .build();

  EXPECT_TRUE(mensaje.getRoomname().has_value());
  EXPECT_EQ(mensaje.getRoomname().value(), "Sala nueva");
}

/**
 * @brief Prueba los metodos setUsernames() y getUsernames() de la clase Mensaje.
 * Comprueba que se asigne correctamente la lista de usernames.
 */
TEST(MensajeTest, AgregaObtieneListaUsernames){
  std::list<std::string> usernames;
  for(int i = 0; i < 10; i++){
    usernames.push_back(nombreAleatorio());
  }
  
  Mensaje mensaje = Mensaje::Builder()
    .setTipo(tipoAleatorio())
    .setUsernames(usernames)
    .build();

  EXPECT_TRUE(mensaje.getUsernames().has_value());
  EXPECT_EQ(mensaje.getUsernames().value(), usernames);
}

/**
 * @brief Prueba el metodo build() de la clase Mensaje.
 * Comprueba que se construya correctamente el mensaje.
 */
TEST(MensajeTest, ConstruirMensaje){
  TipoMensaje tipo = tipoAleatorio();
  OperacionMensaje operacion = operacionAleatoria();
  ResultadoMensaje resultado = resultadoAleatorio();
  std::string username = nombreAleatorio();
  EstatusUsuario estatus = estatusAleatorio();
  std::list<Usuario> users;
  for(int i = 0; i < 10; i++){
    users.push_back(Usuario(nombreAleatorio()));
  }
  std::list<std::string> usernames;
  for(int i = 0; i < 10; i++){
    usernames.push_back(nombreAleatorio());
  }
  
  Mensaje mensaje = Mensaje::Builder()
    .setTipo(tipo)
    .setOperacion(operacion)
    .setResultado(resultado)
    .setUsername(username)
    .setExtra("Contenido extra")
    .setEstatus(estatus)
    .setUsers(users)
    .setText("Mensaje de texto")
    .setRoomname("Sala 1")
    .setUsernames(usernames)
    .build();

  EXPECT_EQ(mensaje.getTipo(), tipo);
    
  EXPECT_TRUE(mensaje.getOperacion().has_value());
  EXPECT_EQ(mensaje.getOperacion().value(), operacion);
    
  EXPECT_TRUE(mensaje.getResultado().has_value());
  EXPECT_EQ(mensaje.getResultado().value(), resultado);
    
  EXPECT_TRUE(mensaje.getUsername().has_value());
  EXPECT_EQ(mensaje.getUsername().value(), username);

  EXPECT_TRUE(mensaje.getExtra().has_value());
  EXPECT_EQ(mensaje.getExtra().value(), "Contenido extra");

  EXPECT_TRUE(mensaje.getEstatus().has_value());
  EXPECT_EQ(mensaje.getEstatus().value(), estatus);

  EXPECT_TRUE(mensaje.getUsers().has_value());
  EXPECT_EQ(mensaje.getUsers().value(), users);

  EXPECT_TRUE(mensaje.getText().has_value());
  EXPECT_EQ(mensaje.getText().value(), "Mensaje de texto");

  EXPECT_TRUE(mensaje.getRoomname().has_value());
  EXPECT_EQ(mensaje.getRoomname().value(), "Sala 1");

  EXPECT_TRUE(mensaje.getUsernames().has_value());
  EXPECT_EQ(mensaje.getUsernames().value(), usernames);

  username = nombreAleatorio();
    
  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::RESPONSE)
    .setOperacion(OperacionMensaje::IDENTIFY)
    .setResultado(ResultadoMensaje::SUCCESS)
    .setExtra(username)
    .build();

  EXPECT_EQ(mensaje.getTipo(), TipoMensaje::RESPONSE);
    
  EXPECT_TRUE(mensaje.getOperacion().has_value());
  EXPECT_EQ(mensaje.getOperacion().value(), OperacionMensaje::IDENTIFY);

  EXPECT_TRUE(mensaje.getResultado().has_value());
  EXPECT_EQ(mensaje.getResultado().value(), ResultadoMensaje::SUCCESS);

  EXPECT_TRUE(mensaje.getExtra().has_value());
  EXPECT_EQ(mensaje.getExtra().value(), username);
}

/**
 * @brief Comprueba que los demas atributos sin asignar sigan en ese estado.
 */
TEST(MensajeTest, CamposVacios){
  Mensaje mensaje = Mensaje::Builder()
    .setTipo(tipoAleatorio())
    .build();

  EXPECT_FALSE(mensaje.getOperacion().has_value());
  EXPECT_FALSE(mensaje.getResultado().has_value());
  EXPECT_FALSE(mensaje.getUsername().has_value());
  EXPECT_FALSE(mensaje.getExtra().has_value());
  EXPECT_FALSE(mensaje.getEstatus().has_value());
  EXPECT_FALSE(mensaje.getUsers().has_value());
  EXPECT_FALSE(mensaje.getText().has_value());
  EXPECT_FALSE(mensaje.getRoomname().has_value());
  EXPECT_FALSE(mensaje.getUsernames().has_value());
}
