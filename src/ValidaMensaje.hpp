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
   */
  static bool validaIdentify(const Mensaje& mensaje);

  /**
   * @brief Valida un mensaje de tipo RESPONSE.
   * @param mensaje El mensaje a verificar.
   */
  static bool validaResponse(const Mensaje& mensaje);

  /**
   * @brief Valida un mensaje de tipo NEW_USER.
   * @param mensaje El mensaje a verificar.
   */
  static bool validaNewUser(const Mensaje& mensaje);

  /**
   * @brief Valida un mensaje de tipo STATUS.
   * @param mensaje El mensaje a verificar.
   */
  static bool validaStatus(const Mensaje& mensaje);

  /**
   * @brief Valida un mensaje de tipo NEW_STATUS.
   * @param mensaje El mensaje a verificar.
   */
  static bool validaNewStatus(const Mensaje& mensaje);

  /**
   * @brief Valida un mensaje de tipo USER_LIST.
   * @param mensaje El mensaje a verificar.
   */
  static bool validaUserList(const Mensaje& mensaje);

  /**
   * @brief Valida un mensaje de tipo TEXT.
   * @param mensaje El mensaje a verificar.
   */
  static bool validaText(const Mensaje& mensaje);

  /**
   * @brief Valida un mensaje de tipo TEXT_FROM.
   * @param mensaje El mensaje a verificar.
   */
  static bool validaTextFrom(const Mensaje& mensaje);

  /**
   * @brief Valida un mensaje de tipo PUBLIC_TEXT.
   * @param mensaje El mensaje a verificar.
   */
  static bool validaPublicText(const Mensaje& mensaje);

  /**
   * @brief Valida un mensaje de tipo PUBLIC_TEXT_FROM.
   * @param mensaje El mensaje a verificar.
   */
  static bool validaPublicTextFrom(const Mensaje& mensaje);

  /**
   * @brief Valida un mensaje de tipo NEW_ROOM.
   * @param mensaje El mensaje a verificar.
   */
  static bool validaNewRoom(const Mensaje& mensaje);

  /**
   * @brief Valida un mensaje de tipo INVITE.
   * @param mensaje El mensaje a verificar.
   */
  static bool validaInvite(const Mensaje& mensaje);

  /**
   * @brief Valida un mensaje de tipo INVITATION.
   * @param mensaje El mensaje a verificar.
   */
  static bool validaInvitation(const Mensaje& mensaje);

  /**
   * @brief Valida un mensaje de tipo JOIN_ROOM.
   * @param mensaje El mensaje a verificar.
   */
  static bool validaJoinRoom(const Mensaje& mensaje);

  /**
   * @brief Valida un mensaje de tipo JOINED_ROOM.
   * @param mensaje El mensaje a verificar.
   */
  static bool validaJoinedRoom(const Mensaje& mensaje);

  /**
   * @brief Valida un mensaje de tipo ROOM_USERS.
   * @param mensaje El mensaje a verificar.
   */
  static bool validaRoomUsers(const Mensaje& mensaje);

  /**
   * @brief Valida un mensaje de tipo ROOM_USER_LIST.
   * @param mensaje El mensaje a verificar.
   */
  static bool validaRoomUserList(const Mensaje& mensaje);

  /**
   * @brief Valida un mensaje de tipo ROOM_TEXT.
   * @param mensaje El mensaje a verificar.
   */
  static bool validaRoomText(const Mensaje& mensaje);

  /**
   * @brief Valida un mensaje de tipo ROOM_TEXT_FROM.
   * @param mensaje El mensaje a verificar.
   */
  static bool validaRoomTextFrom(const Mensaje& mensaje);

  /**
   * @brief Valida un mensaje de tipo LEAVE_ROOM.
   * @param mensaje El mensaje a verificar.
   */
  static bool validaLeaveRoom(const Mensaje& mensaje);

  /**
   * @brief Valida un mensaje de tipo LEFT_ROOM.
   * @param mensaje El mensaje a verificar.
   */
  static bool validaLeftRoom(const Mensaje& mensaje);

  /**
   * @brief Valida un mensaje de tipo DISCONNECTED.
   * @param mensaje El mensaje a verificar.
   */
  static bool validaDisconnected(const Mensaje& mensaje);
  
public:

  /**
   * @brief Valida un mensaje.
   * @param mensaje El mensaje a verificar.
   */
  static bool valida(const Mensaje& mensaje);
  
};

#endif
