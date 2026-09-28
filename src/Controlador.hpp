/**
 * @file Controlador.hpp
 * @brief Header de la clase Controlador.
 */

#ifndef CONTROLADOR_HPP
#define CONTROLADOR_HPP

#include <QObject>
#include <memory>
#include <iostream>
#include <QVariantMap>
#include <QStringList>
#include <list>
#include "Mensaje.hpp"
#include "Cliente.hpp"

/**
 * @class Controlador
 * @brief Comunicación entre el cliente y la interfaz.
 */
class Controlador : public QObject{
  Q_OBJECT

private:
  std::unique_ptr<Cliente> cliente; /*!< Cliente con el que se comunicará el controlador */

  /**
   * @brief Procesa los mensajes recibidos por el cliente
   * @param mensaje El mensaje recibido.
   */
  void mensajeRecibido(const Mensaje& mensaje);

  /**
   * @brief Procesa los mensaje de tipo RESPONSE.
   * @param mensaje El mensaje recibido.
   */
  void manejaResponse(const Mensaje& mensaje);

public:
  
  /**
   * @brief Constructor del controlador.
   * @param ipServidor La IP del servidor al cual conectarse.
   * @param puerto El puerto donde se encuentra el servidor.
   * @param parent El padre del controlador dentro de Qt.
   */
  explicit Controlador(std::string ipServidor, int puerto, QObject* parent = nullptr);

  /**
   * @brief Destructor del controlador.
   */
  ~Controlador();

  /**
   * @brief Hace que el cliente se identifique en el servidor.
   * @param nombre El nombre con el cual se identificará.
   */
  Q_INVOKABLE void identificarse(const QString& nombre);

  /**
   * @brief Hace que el cliente cambie el estatus del usuario.
   * @param estatus El nuevo estatus.
   */
  Q_INVOKABLE void cambiarEstatus(int estatus);

  /**
   * @brief Hace que el cliente solicite la lista de usuarios conectados.
   */
  Q_INVOKABLE void obtenerUsuarios();

  /**
   * @brief Hace que el cliente envíe un texto privado
   * @param usuario El usuario al cual se le enviara el texto.
   * @param texto El texto que se enviara.
   */
  Q_INVOKABLE void enviarTexto(const QString& usuario, const QString& texto);

  /**
   * @brief Hace que el cliente envíe un texto público.
   * @param text El texto que se enviara.
   */
  Q_INVOKABLE void enviarTextoPublico(const QString& texto);

  /**
   * @brief Hace que el cliente envie un texto a una sala.
   * @param sala La sala a la cual se enviara el texto.
   * @param texto El texto que se enviara.
   */
  Q_INVOKABLE void enviarTextoSala(const QString& sala, const QString& texto);

  /**
   * @brief Hace que el cliente solicite crear una sala.
   * @param sala El nombre de la sala a crear.
   */
  Q_INVOKABLE void crearSala(const QString& sala);

  /**
   * @brief Hace que el cliente solicite la lista de usuarios de una sala.
   * @param sala El nombre de sala de donde se obtendra la lista.
   */
  Q_INVOKABLE void obtenerUsuariosSala(const QString& sala);

  /**
   * @brief Hace que el cliente invite a varios usuarios a una sala.
   * @param lista La lista de usuarios a invitar.
   * @param sala La sala a la cual seran invitados.
   */
  Q_INVOKABLE void invitar(const QStringList& lista, const QString& sala);

  /**
   * @brief Hace que el cliente acepte una invitación a una sala.
   * @param sala La sala a la cual se unira.
   */
  Q_INVOKABLE void aceptarInvitacion(const QString& sala);

  /**
   * @brief Hace que el cliente salga de una sala.
   * @param sala La sala de la cual el cliente saldra.
   */
  Q_INVOKABLE void salirSala(const QString& sala);

  /**
   * @brief Hace que el cliente se desconecte.
   */
  Q_INVOKABLE void desconectar();

signals:

  /**
   * @brief Notifica que el cliente se identifico con exito,
   * @param extra El nombre de usuario con el que se identifico.
   */
  void identificacionExitosa(const QString& extra);

  /**
   * @brief Notifica que ya hay un usuario con el nombre enviado.
   * @param extra El nombre del usuario que ya se encuentra en el servidor.
   */
  void identificacionYaExiste(const QString& extra);

  /**
   * @brief Notifica que un nuevo usuario se conecto al servidor.
   * @param usuario El nombre del usuario que se conecto.
   */
  void nuevoUsuario(const QString& usuario);

  /**
   * @brief Envia a la interfaz la lista de usuarios solicitada.
   * @param lista La lista de usuarios recibida.
   */
  void listaUsuarios(const QVariantMap& lista);

  /**
   * @brief Notifica que un usuario cambio su estatus.
   * @param usuario El usuario que cambio su estatus.
   * @param estatus El nuevo estatus del usuario.
   */
  void nuevoEstatus(const QString& usuario, int estatus);

  /**
   * @brief Notifica que se recibio un texto privado.
   * @param usuario El usuario que envio el texto.
   * @param texto El texto enviado.
   */
  void mensajePrivado(const QString& usuario, const QString& texto);

  /**
   * @brief Notifica que un usuario no se encuentra conectado.
   * @param usuario El usuario no encontrado.
   */
  void usuarioNoEncontrado(const QString& usuario);

  /**
   * @brief Notifica que se recibio un texto publico.
   * @param usuario El usuario que envio el texto.
   * @param texto El texto enviado.
   */
  void mensajePublico(const QString& usuario, const QString& texto);

  /**
   * @brief Notifica que un usuario se desconecto.
   * @param usuario El usuario que se desconecto.
   */
  void desconectado(const QString& usuario);

  /**
   * @brief Notifica que se logro crear una sala con exito.
   * @param sala La sala que se logro crear.
   */
  void salaCreada(const QString& sala);

  /**
   * @brief Notifica que la sala ya existe.
   * @param sala La sala que ya existe.
   */
  void salaYaExiste(const QString& sala);

  /**
   * @brief Envia a la interfaz la lista de usuarios de una sala.
   * @param lista La lista recibida.
   */
  void listaSala(const QVariantMap& lista);

  /**
   * @brief Notifica que se recibio una invitación a una sala.
   * @param usuario El usuario que envió la invitación
   * @param sala La sala a la que se invito.
   */
  void invitacionRecibida(const QString& usuario, const QString& sala);

  /**
   * @brief Notifica que la sala no existe.
   * @param sala La sala que no existe.
   */
  void salaNoExiste(const QString& sala);

  /**
   * @brief Notifica que se acepto la invitación correctamente.
   * @param sala La sala a la que se acepto unirse.
   */
  void invitacionAceptada(const QString& sala);

  /**
   * @brief Notifica que un usuario se unio a una sala.
   * @param sala La sala a la que se unió.
   * @param usuario El usuario que se unió.
   */
  void nuevoUsuarioSala(const QString& sala, const QString& usuario);

  /**
   * @brief Notifica que el usuario no esta invitado a una sala.
   * @param sala La sala en la que no esta invitado.
   */
  void noInvitado(const QString& sala);

  /**
   * @brief Notifica que el usuario no esta unido a una sala.
   * @param sala La sala en la que no esta unido.
   */
  void noUnido(const QString& sala);

  /**
   * @brief Notifica que se recibio un texto a una sala.
   * @param sala La sala a la que se envio el texto.
   * @param usuario El usuario que envio el texto.
   * @param texto El texto enviado.
   */
  void mensajeSala(const QString& sala, const QString& usuario, const QString& texto);

  /**
   * @brief Notifica que un usuario salio de una sala.
   * @param sala La sala de la cual salio.
   * @param usuario El usuario que salio de la sala.
   */
  void salioDeSala(const QString& sala, const QString& usuario);
};

#endif
