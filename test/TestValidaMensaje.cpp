/**
 * @file TestValidaMensaje.cpp
 * @brief Pruebas unitarias para la clase ValidaMensaje.
 */

#include <gtest/gtest.h>
#include <string>
#include <list>
#include <unordered_map>
#include "Usuario.hpp"
#include "EstatusUsuario.hpp"
#include "Mensaje.hpp"
#include "TipoMensaje.hpp"
#include "OperacionMensaje.hpp"
#include "ResultadoMensaje.hpp"
#include "ValidaMensaje.hpp"

/**
 * @brief Prueba el metodo validaIdentify() de la clase ValidaMensaje.
 * Comprueba que se valide correctamente el mensaje de tipo IDENTIFY.
 */
TEST(ValidaMensajeTest, ValidaIdentify){
  Mensaje mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::IDENTIFY)
    .setUsername("Alice")
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::IDENTIFY)
    .setUsername("Bob")
    .setText("Texto no necesario")
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::IDENTIFY)
    .setText("Texto no necesario")
    .build();

  EXPECT_FALSE(ValidaMensaje::valida(mensaje));
}

/**
 * @brief Prueba el metodo validaResponse() de la clase ValidaMensaje.
 * Comprueba que se valide correctamente el mensaje de tipo RESPONSE.
 */
TEST(ValidaMensajeTest, ValidaResponse){
  Mensaje mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::RESPONSE)
    .setOperacion(OperacionMensaje::IDENTIFY)
    .setResultado(ResultadoMensaje::SUCCESS)
    .setExtra("Alice")
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::RESPONSE)
    .setOperacion(OperacionMensaje::IDENTIFY)
    .setResultado(ResultadoMensaje::SUCCESS)
    .build();

  EXPECT_FALSE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::RESPONSE)
    .setOperacion(OperacionMensaje::INVALID)
    .setResultado(ResultadoMensaje::NOT_IDENTIFIED)
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));
  
  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::RESPONSE)
    .setOperacion(OperacionMensaje::INVALID)
    .setResultado(ResultadoMensaje::NOT_IDENTIFIED)
    .setExtra("Bob")
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::RESPONSE)
    .setOperacion(OperacionMensaje::INVALID)
    .build();

  EXPECT_FALSE(ValidaMensaje::valida(mensaje));
}

/**
 * @brief Prueba el metodo validaNewUser() de la clase ValidaMensaje.
 * Comprueba que se valide correctamente el mensaje de tipo NEW_USER.
 */
TEST(ValidaMensajeTest, ValidaNewUser){
  Mensaje mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::NEW_USER)
    .setUsername("Alice")
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::NEW_USER)
    .setUsername("Alice")
    .setText("Texto no necesario")
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::NEW_USER)
    .build();
  
  EXPECT_FALSE(ValidaMensaje::valida(mensaje));
  
  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::NEW_USER)
    .setText("Texto no necesario")
    .build();

  EXPECT_FALSE(ValidaMensaje::valida(mensaje));
}

/**
 * @brief Prueba el metodo validaStatus() de la clase ValidaMensaje.
 * Comprueba que se valide correctamente el mensaje de tipo STATUS.
 */
TEST(ValidaMensajeTest, ValidaStatus){
  Mensaje mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::STATUS)
    .setEstatus(EstatusUsuario::AWAY)
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::STATUS)
    .setEstatus(EstatusUsuario::AWAY)
    .setText("Texto no necesario")
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::STATUS)
    .build();

  EXPECT_FALSE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::STATUS)
    .setText("Texto no necesario")
    .build();
  
 EXPECT_FALSE(ValidaMensaje::valida(mensaje));
}

/**
 * @brief Prueba el metodo validaNewStatus() de la clase ValidaMensaje.
 * Comprueba que se valide correctamente el mensaje de tipo NEW_STATUS.
 */
TEST(ValidaMensajeTest, ValidaNewStatus){
  Mensaje mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::NEW_STATUS)
    .setUsername("Alice")
    .setEstatus(EstatusUsuario::AWAY)
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::NEW_STATUS)
    .setUsername("Alice")
    .setEstatus(EstatusUsuario::ACTIVE)
    .setExtra("Texto no necesario")
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::NEW_STATUS)
    .setUsername("Alice")
    .setExtra("Texto no necesario")
    .build();

  EXPECT_FALSE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::NEW_STATUS)
    .setEstatus(EstatusUsuario::BUSY)
    .setExtra("Texto no necesario")
    .build();

  EXPECT_FALSE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::NEW_STATUS)
    .setEstatus(EstatusUsuario::AWAY)
    .build();

  EXPECT_FALSE(ValidaMensaje::valida(mensaje));
}

/**
 * @brief Prueba el metodo validaUsers() de la clase ValidaMensaje.
 * Comprueba que se valide correctamente el mensaje de tipo USERS.
 */
TEST(ValidaMensajeTest, ValidaUsers){
  Mensaje mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::USERS)
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::USERS)
    .setExtra("Texto no necesario")
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));
}

/**
 * @brief Prueba el metodo validaUserList() de la clase ValidaMensaje.
 * Comprueba que se valide correctamente el mensaje de tipo USER_LIST.
 */
TEST(ValidaMensajeTest, ValidaUserList){
  std::unordered_map<std::string, EstatusUsuario> users =
    {{"Alice", EstatusUsuario::ACTIVE}, {"Bob", EstatusUsuario::BUSY}};
  
  Mensaje mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::USER_LIST)
    .setUsers(users)
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::USER_LIST)
    .setUsers(users)
    .setExtra("Texto no necesario")
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::USER_LIST)
    .setExtra("Texto no necesario")
    .build();

  EXPECT_FALSE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::USER_LIST)
    .build();
  
  EXPECT_FALSE(ValidaMensaje::valida(mensaje));
}

/**
 * @brief Prueba el metodo validaText() de la clase ValidaMensaje.
 * Comprueba que se valide correctamente el mensaje de tipo TEXT.
 */
TEST(ValidaMensajeTest, ValidaText){
  Mensaje mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::TEXT)
    .setUsername("Alice")
    .setText("Texto necesario")
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::TEXT)
    .setUsername("Alice")
    .setText("Texto necesario")
    .setExtra("Texto no necesario")
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::TEXT)
    .setUsername("Alice")
    .setExtra("Texto no necesario")
    .build();

  EXPECT_FALSE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::TEXT)
    .setText("Texto necesario")
    .build();

  EXPECT_FALSE(ValidaMensaje::valida(mensaje));
}

/**
 * @brief Prueba el metodo validaTextFrom() de la clase ValidaMensaje.
 * Comprueba que se valide correctamente el mensaje de tipo TEXT_FROM.
 */
TEST(ValidaMensajeTest, ValidaTextFrom){
  Mensaje mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::TEXT_FROM)
    .setUsername("Alice")
    .setText("Texto necesario")
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::TEXT_FROM)
    .setUsername("Alice")
    .setText("Texto necesario")
    .setExtra("Texto no necesario")
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::TEXT_FROM)
    .setUsername("Alice")
    .setExtra("Texto no necesario")
    .build();

  EXPECT_FALSE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::TEXT_FROM)
    .setText("Texto necesario")
    .build();

  EXPECT_FALSE(ValidaMensaje::valida(mensaje));
}

/**
 * @brief Prueba el metodo validaPublicText() de la clase ValidaMensaje.
 * Comprueba que se valide correctamente el mensaje de tipo PUBLIC_TEXT.
 */
TEST(ValidaMensajeTest, ValidaPublicText){
  Mensaje mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::PUBLIC_TEXT)
    .setText("Texto necesario")
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::PUBLIC_TEXT)
    .setText("Texto necesario")
    .setExtra("Texto no necesario")
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::PUBLIC_TEXT)
    .setExtra("Texto no necesario")
    .build();

  EXPECT_FALSE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::PUBLIC_TEXT)
    .build();

  EXPECT_FALSE(ValidaMensaje::valida(mensaje));
}

/**
 * @brief Prueba el metodo validaPublicTextFrom() de la clase ValidaMensaje.
 * Comprueba que se valide correctamente el mensaje de tipo PUBLIC_TEXT_FROM.
 */
TEST(ValidaMensajeTest, ValidaPublicTextFrom){
  Mensaje mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::PUBLIC_TEXT_FROM)
    .setUsername("Alice")
    .setText("Texto necesario")
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::PUBLIC_TEXT_FROM)
    .setUsername("Alice")
    .setText("Texto necesario")
    .setExtra("Texto no necesario")
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::PUBLIC_TEXT_FROM)
    .setUsername("Alice")
    .setExtra("Texto no necesario")
    .build();

  EXPECT_FALSE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::PUBLIC_TEXT_FROM)
    .setText("Texto necesario")
    .build();

  EXPECT_FALSE(ValidaMensaje::valida(mensaje));
}

/**
 * @brief Prueba el metodo validaNewRoom() de la clase ValidaMensaje.
 * Comprueba que se valide correctamente el mensaje de tipo NEW_ROOM.
 */
TEST(ValidaMensajeTest, ValidaNewRoom){
  Mensaje mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::NEW_ROOM)
    .setRoomname("Sala 1")
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::NEW_ROOM)
    .setRoomname("Sala 1")
    .setExtra("Texto no necesario")
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::NEW_ROOM)
    .setExtra("Texto no necesario")
    .build();
  
  EXPECT_FALSE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::NEW_ROOM)
    .build();
  
  EXPECT_FALSE(ValidaMensaje::valida(mensaje));
}

/**
 * @brief Prueba el metodo validaInvite() de la clase ValidaMensaje.
 * Comprueba que se valide correctamente el mensaje de tipo INVITE.
 */
TEST(ValidaMensajeTest, ValidaInvite){
  std::list<std::string> nombres = {"Alice", "Bob"};
  
  Mensaje mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::INVITE)
    .setRoomname("Sala 1")
    .setUsernames(nombres)
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::INVITE)
    .setRoomname("Sala 1")
    .setUsernames(nombres)
    .setExtra("Texto no necesario")
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::INVITE)
    .setRoomname("Sala 1")
    .setExtra("Texto no necesario")
    .build();

  EXPECT_FALSE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::INVITE)
    .setUsernames(nombres)
    .build();

  EXPECT_FALSE(ValidaMensaje::valida(mensaje));
}

/**
 * @brief Prueba el metodo validaInvitation() de la clase ValidaMensaje.
 * Comprueba que se valide correctamente el mensaje de tipo INVITATION.
 */
TEST(ValidaMensajeTest, ValidaInvitation){
  Mensaje mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::INVITATION)
    .setUsername("Alice")
    .setRoomname("Sala 1")
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::INVITATION)
    .setUsername("Alice")
    .setRoomname("Sala 1")
    .setExtra("Texto no necesario")
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::INVITATION)
    .setUsername("Alice")
    .setExtra("Texto no necesario")
    .build();

  EXPECT_FALSE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::INVITATION)
    .setRoomname("Sala 1")
    .build();

  EXPECT_FALSE(ValidaMensaje::valida(mensaje));
}

/**
 * @brief Prueba el metodo validaJoinRoom() de la clase ValidaMensaje.
 * Comprueba que se valide correctamente el mensaje de tipo JOIN_ROOM.
 */
TEST(ValidaMensajeTest, ValidaJoinRoom){
  Mensaje mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::JOIN_ROOM)
    .setRoomname("Sala 1")
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::JOIN_ROOM)
    .setRoomname("Sala 1")
    .setExtra("Texto no necesario")
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::JOIN_ROOM)
    .setExtra("Texto no necesario")
    .build();

  EXPECT_FALSE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::JOIN_ROOM)
    .build();

  EXPECT_FALSE(ValidaMensaje::valida(mensaje));
}

/**
 * @brief Prueba el metodo validaJoinedRoom() de la clase ValidaMensaje.
 * Comprueba que se valide correctamente el mensaje de tipo JOINED_ROOM.
 */
TEST(ValidaMensajeTest, ValidaJoinedRoom){
  Mensaje mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::JOINED_ROOM)
    .setRoomname("Sala 1")
    .setUsername("Alice")
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::JOINED_ROOM)
    .setRoomname("Sala 1")
    .setUsername("Alice")
    .setExtra("Texto no necesario")
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::JOINED_ROOM)
    .setRoomname("Sala 1")
    .setExtra("Texto no necesario")
    .build();

  EXPECT_FALSE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::JOINED_ROOM)
    .build();

  EXPECT_FALSE(ValidaMensaje::valida(mensaje));
}

/**
 * @brief Prueba el metodo validaRoomUsers() de la clase ValidaMensaje.
 * Comprueba que se valide correctamente el mensaje de tipo ROOM_USERS.
 */
TEST(ValidaMensajeTest, ValidaRoomUsers){
  Mensaje mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::ROOM_USERS)
    .setRoomname("Sala 1")
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::ROOM_USERS)
    .setRoomname("Sala 1")
    .setExtra("Texto no necesario")
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::ROOM_USERS)
    .setExtra("Texto no necesario")
    .build();

  EXPECT_FALSE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::ROOM_USERS)
    .build();

  EXPECT_FALSE(ValidaMensaje::valida(mensaje));
}

/**
 * @brief Prueba el metodo validaUserList() de la clase ValidaMensaje.
 * Comprueba que se valide correctamente el mensaje de tipo UserList.
 */
TEST(ValidaMensajeTest, ValidaRoomUserList){
  std::unordered_map<std::string, EstatusUsuario> users =
    {{"Alice", EstatusUsuario::ACTIVE}, {"Bob", EstatusUsuario::BUSY}};
    
  Mensaje mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::ROOM_USER_LIST)
    .setRoomname("Sala 1")
    .setUsers(users)
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::ROOM_USER_LIST)
    .setRoomname("Sala 1")
    .setUsers(users)
    .setExtra("Texto no necesario")
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::ROOM_USER_LIST)
    .setRoomname("Sala 1")
    .setExtra("Texto no necesario")
    .build();

  EXPECT_FALSE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::ROOM_USER_LIST)
    .build();

  EXPECT_FALSE(ValidaMensaje::valida(mensaje));
}

/**
 * @brief Prueba el metodo validaRoomText() de la clase ValidaMensaje.
 * Comprueba que se valide correctamente el mensaje de tipo ROOM_TEXT.
 */
TEST(ValidaMensajeTest, ValidaRoomText){
  Mensaje mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::ROOM_TEXT)
    .setRoomname("Sala 1")
    .setText("Texto necesario")
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::ROOM_TEXT)
    .setRoomname("Sala 1")
    .setText("Texto necesario")
    .setExtra("Texto no necesario")
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::ROOM_TEXT)
    .setRoomname("Sala 1")
    .setExtra("Texto no necesario")
    .build();

  EXPECT_FALSE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::ROOM_TEXT)
    .build();

  EXPECT_FALSE(ValidaMensaje::valida(mensaje));
}

/**
 * @brief Prueba el metodo validaRoomTextFrom() de la clase ValidaMensaje.
 * Comprueba que se valide correctamente el mensaje de tipo ROOM_TEXT_FROM.
 */
TEST(ValidaMensajeTest, ValidaRoomTextFrom){
  Mensaje mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::ROOM_TEXT_FROM)
    .setRoomname("Sala 1")
    .setUsername("Alice")
    .setText("Texto necesario")
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::ROOM_TEXT_FROM)
    .setRoomname("Sala 1")
    .setUsername("Alice")
    .setText("Texto necesario")
    .setExtra("Texto no necesario")
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::ROOM_TEXT_FROM)
    .setRoomname("Sala 1")
    .setUsername("Alice")
    .setExtra("Texto no necesario")
    .build();

  EXPECT_FALSE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::ROOM_TEXT_FROM)
    .build();

  EXPECT_FALSE(ValidaMensaje::valida(mensaje));
}

/**
 * @brief Prueba el metodo validaLeaveRoom() de la clase ValidaMensaje.
 * Comprueba que se valide correctamente el mensaje de tipo LEAVE_ROOM.
 */
TEST(ValidaMensajeTest, ValidaLeaveRoom){
  Mensaje mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::LEAVE_ROOM)
    .setRoomname("Sala 1")
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::LEAVE_ROOM)
    .setRoomname("Sala 1")
    .setExtra("Texto no necesario")
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::LEAVE_ROOM)
    .setExtra("Texto no necesario")
    .build();

  EXPECT_FALSE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::LEAVE_ROOM)
    .build();

  EXPECT_FALSE(ValidaMensaje::valida(mensaje));
}

/**
 * @brief Prueba el metodo validaLeftRoom() de la clase ValidaMensaje.
 * Comprueba que se valide correctamente el mensaje de tipo LEFT_ROOM.
 */
TEST(ValidaMensajeTest, ValidaLeftRoom){
  Mensaje mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::LEFT_ROOM)
    .setRoomname("Sala 1")
    .setUsername("Alice")
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::LEFT_ROOM)
    .setRoomname("Sala 1")
    .setUsername("Alice")
    .setExtra("Texto no necesario")
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::LEFT_ROOM)
    .setRoomname("Sala 1")
    .setExtra("Texto no necesario")
    .build();

  EXPECT_FALSE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::LEFT_ROOM)
    .build();

  EXPECT_FALSE(ValidaMensaje::valida(mensaje));
}

/**
 * @brief Prueba el metodo validaDisconnect() de la clase ValidaMensaje.
 * Comprueba que se valide correctamente el mensaje de tipo DISCONNECT.
 */
TEST(ValidaMensajeTest, ValidaDisconnect){
  Mensaje mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::DISCONNECT)
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::DISCONNECT)
    .setExtra("Texto no necesario")
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));
}

/**
 * @brief Prueba el metodo validaDisconnected() de la clase ValidaMensaje.
 * Comprueba que se valide correctamente el mensaje de tipo DISCONNECTED.
 */
TEST(ValidaMensajeTest, ValidaDisconnected){
  Mensaje mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::DISCONNECTED)
    .setUsername("Alice")
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::DISCONNECTED)
    .setUsername("Alice")
    .setExtra("Texto no necesario")
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::DISCONNECTED)
    .setExtra("Texto no necesario")
    .build();

  EXPECT_FALSE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::DISCONNECTED)
    .build();

  EXPECT_FALSE(ValidaMensaje::valida(mensaje));
}

/**
 * @brief Prueba el metodo valida() de la clase ValidaMensaje.
 * Comprueba que se valide correctamente los mensajes.
 */
TEST(ValidaMensajeTest, Valida){
  Mensaje mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::IDENTIFY)
    .setUsername("Alice")
    .build();

  EXPECT_TRUE(ValidaMensaje::valida(mensaje));

  mensaje = Mensaje::Builder()
    .setTipo(TipoMensaje::IDENTIFY)
    .build();

  EXPECT_FALSE(ValidaMensaje::valida(mensaje));
}
