/**
 * @file ValidaMensaje.hpp
 * @brief Header de la clase ValidaMensaje.hpp
 */

#ifndef VALIDAMENSAJE_HPP
#define VALIDAMENSAJE_HPP

#include "Mensaje.hpp"

/**
 * @class ValidaMensaje
 * @brief Verifica si un Mensaje es valido.
 */
class ValidaMensaje{
private:
  // Constructor privado, no permite instanciar la clase.
  ValidaMensaje() = delete;

  /**
   * @brief Valida un mensaje de tipo IDENTIFY.
   * @param mensaje El mensaje a verificar.
   * @return Si el mensaje es valido.
   */
  static bool validaIdentify(const Mensaje& mensaje);

  /**
   * @brief Valida un mensaje de tipo RESPONSE.
   * @param mensaje El mensaje a verificar.
   * @return Si el mensaje es valido.
   */
  static bool validaResponse(const Mensaje& mensaje);

  /**
   * @brief Valida un mensaje de tipo NEW_USER.
   * @param mensaje El mensaje a verificar.
   * @return Si el mensaje es valido.
   */
  static bool validaNewUser(const Mensaje& mensaje);

  /**
   * @brief Valida un mensaje de tipo STATUS.
   * @param mensaje El mensaje a verificar.
   * @return Si el mensaje es valido.
   */
  static bool validaStatus(const Mensaje& mensaje);

  /**
   * @brief Valida un mensaje de tipo NEW_STATUS.
   * @param mensaje El mensaje a verificar.
   * @return Si el mensaje es valido.
   */
  static bool validaNewStatus(const Mensaje& mensaje);

  /**
   * @brief Valida un mensaje de tipo USER_LIST.
   * @param mensaje El mensaje a verificar.
   * @return Si el mensaje es valido.
   */
  static bool validaUserList(const Mensaje& mensaje);

  /**
   * @brief Valida un mensaje de tipo TEXT.
   * @param mensaje El mensaje a verificar.
   * @return Si el mensaje es valido.
   */
  static bool validaText(const Mensaje& mensaje);

  /**
   * @brief Valida un mensaje de tipo TEXT_FROM.
   * @param mensaje El mensaje a verificar.
   * @return Si el mensaje es valido.
   */
  static bool validaTextFrom(const Mensaje& mensaje);

  /**
   * @brief Valida un mensaje de tipo PUBLIC_TEXT.
   * @param mensaje El mensaje a verificar.
   * @return Si el mensaje es valido.
   */
  static bool validaPublicText(const Mensaje& mensaje);

  /**
   * @brief Valida un mensaje de tipo PUBLIC_TEXT_FROM.
   * @param mensaje El mensaje a verificar.
   * @return Si el mensaje es valido.
   */
  static bool validaPublicTextFrom(const Mensaje& mensaje);

  /**
   * @brief Valida un mensaje de tipo NEW_ROOM.
   * @param mensaje El mensaje a verificar.
   * @return Si el mensaje es valido.
   */
  static bool validaNewRoom(const Mensaje& mensaje);

  /**
   * @brief Valida un mensaje de tipo INVITE.
   * @param mensaje El mensaje a verificar.
   * @return Si el mensaje es valido.
   */
  static bool validaInvite(const Mensaje& mensaje);

  /**
   * @brief Valida un mensaje de tipo INVITATION.
   * @param mensaje El mensaje a verificar.
   * @return Si el mensaje es valido.
   */
  static bool validaInvitation(const Mensaje& mensaje);

  /**
   * @brief Valida un mensaje de tipo JOIN_ROOM.
   * @param mensaje El mensaje a verificar.
   * @return Si el mensaje es valido.
   */
  static bool validaJoinRoom(const Mensaje& mensaje);

  /**
   * @brief Valida un mensaje de tipo JOINED_ROOM.
   * @param mensaje El mensaje a verificar.
   * @return Si el mensaje es valido.
   */
  static bool validaJoinedRoom(const Mensaje& mensaje);

  /**
   * @brief Valida un mensaje de tipo ROOM_USERS.
   * @param mensaje El mensaje a verificar.
   * @return Si el mensaje es valido.
   */
  static bool validaRoomUsers(const Mensaje& mensaje);

  /**
   * @brief Valida un mensaje de tipo ROOM_USER_LIST.
   * @param mensaje El mensaje a verificar.
   * @return Si el mensaje es valido.
   */
  static bool validaRoomUserList(const Mensaje& mensaje);

  /**
   * @brief Valida un mensaje de tipo ROOM_TEXT.
   * @param mensaje El mensaje a verificar.
   * @return Si el mensaje es valido.
   */
  static bool validaRoomText(const Mensaje& mensaje);

  /**
   * @brief Valida un mensaje de tipo ROOM_TEXT_FROM.
   * @param mensaje El mensaje a verificar.
   * @return Si el mensaje es valido.
   */
  static bool validaRoomTextFrom(const Mensaje& mensaje);

  /**
   * @brief Valida un mensaje de tipo LEAVE_ROOM.
   * @param mensaje El mensaje a verificar.
   * @return Si el mensaje es valido.
   */
  static bool validaLeaveRoom(const Mensaje& mensaje);

  /**
   * @brief Valida un mensaje de tipo LEFT_ROOM.
   * @param mensaje El mensaje a verificar.
   * @return Si el mensaje es valido.
   */
  static bool validaLeftRoom(const Mensaje& mensaje);

  /**
   * @brief Valida un mensaje de tipo DISCONNECTED.
   * @param mensaje El mensaje a verificar.
   * @return Si el mensaje es valido.
   */
  static bool validaDisconnected(const Mensaje& mensaje);
  
public:

  /**
   * @brief Valida un mensaje.
   * @param mensaje El mensaje a verificar.
   * @return Si el mensaje es valido.
   */
  static bool valida(const Mensaje& mensaje);
  
};

#endif
