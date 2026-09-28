/**
 * @file Controlador.cpp
 * @brief Implementación de la clase Controlador.
 */ 

#include <QObject>
#include <memory>
#include <iostream>
#include <QVariantMap>
#include <QStringList>
#include <list>
#include "Mensaje.hpp"
#include "Cliente.hpp"
#include "Controlador.hpp"

Controlador::Controlador(std::string ipServidor, int puerto, QObject* parent)
    : QObject(parent)
{
    cliente = std::make_unique<Cliente>(ipServidor, puerto);

    cliente->agregaEscucha([this](const Mensaje& mensaje){
      mensajeRecibido(mensaje);
    });
}

Controlador::~Controlador(){
  cliente->desconectar();
}

void Controlador::identificarse(const QString& nombre){
  cliente->identificarse(nombre.toStdString());
}

void Controlador::cambiarEstatus(int estatus){
  switch(estatus){
  case 0:
    cliente->cambiarEstatus(EstatusUsuario::ACTIVE);
    break;
  case 1:
    cliente->cambiarEstatus(EstatusUsuario::AWAY);
    break;
  case 2:
    cliente->cambiarEstatus(EstatusUsuario::BUSY);
    break;
  default:
    break;
  }
}

void Controlador::obtenerUsuarios(){
  cliente->obtenerUsuarios();
}

void Controlador::enviarTexto(const QString& usuario, const QString& texto){
  cliente->enviarTexto(usuario.toStdString(), texto.toStdString());
}

void Controlador::enviarTextoPublico(const QString& texto){
  cliente->enviarTextoPublico(texto.toStdString());
}

void Controlador::crearSala(const QString& sala){
  cliente->crearSala(sala.toStdString());
}

void Controlador::enviarTextoSala(const QString& sala, const QString& texto){
  cliente->enviarTextoSala(sala.toStdString(), texto.toStdString());
}

void Controlador::obtenerUsuariosSala(const QString& sala){
  cliente->obtenerUsuariosSala(sala.toStdString());
}

void Controlador::invitar(const QStringList& lista, const QString& sala){
  std::list<std::string> usuarios;

  for(const QString& usuario : lista)
    usuarios.push_back(usuario.toStdString());

  cliente->invitar(usuarios, sala.toStdString());
}

void Controlador::aceptarInvitacion(const QString& sala){
  cliente->entrarSala(sala.toStdString());
}

void Controlador::salirSala(const QString& sala){
  cliente->salirSala(sala.toStdString());
}

void Controlador::desconectar(){
  cliente->desconectar();
}

void Controlador::mensajeRecibido(const Mensaje& mensaje){
  TipoMensaje tipo = mensaje.getTipo();
  
  switch(tipo){
  case TipoMensaje::RESPONSE:
    manejaResponse(mensaje);
    break;

    
  case TipoMensaje::NEW_USER:
    emit nuevoUsuario(QString::fromStdString(mensaje.getUsername().value()));
    break;

    
  case TipoMensaje::NEW_STATUS:
    emit nuevoEstatus(QString::fromStdString(mensaje.getUsername().value()),
		      static_cast<int>(mensaje.getEstatus().value()));
    break;

    
  case TipoMensaje::USER_LIST:{
    QVariantMap diccionario;
    std::unordered_map<std::string, EstatusUsuario> usuarios = mensaje.getUsers().value();
    
    for(const auto& [nombre, estatus] : usuarios){
      diccionario[QString::fromStdString(nombre)] = static_cast<int>(estatus);
    }

    emit listaUsuarios(diccionario);
    break;
  }

    
  case TipoMensaje::TEXT_FROM:
    emit mensajePrivado(QString::fromStdString(mensaje.getUsername().value()),
			QString::fromStdString(mensaje.getText().value()));
    break;

    
  case TipoMensaje::PUBLIC_TEXT_FROM:
    emit mensajePublico(QString::fromStdString(mensaje.getUsername().value()),
			QString::fromStdString(mensaje.getText().value()));
    break;

    
  case TipoMensaje::ROOM_USER_LIST:{
    QVariantMap diccionarioSala;
    std::unordered_map<std::string, EstatusUsuario> usuarios = mensaje.getUsers().value();

    for(const auto& [nombre, estatus] : usuarios){
      diccionarioSala[QString::fromStdString(nombre)] = static_cast<int>(estatus);
    }
    
    emit listaSala(diccionarioSala);
    break;
  }

    
  case TipoMensaje::INVITATION:
    emit invitacionRecibida(QString::fromStdString(mensaje.getUsername().value()),
			    QString::fromStdString(mensaje.getRoomname().value()));
    break;

    
  case TipoMensaje::JOINED_ROOM:
    emit nuevoUsuarioSala(QString::fromStdString(mensaje.getRoomname().value()),
			  QString::fromStdString(mensaje.getUsername().value()));
    break;

    
  case TipoMensaje::ROOM_TEXT_FROM:
    emit mensajeSala(QString::fromStdString(mensaje.getRoomname().value()),
		     QString::fromStdString(mensaje.getUsername().value()),
		     QString::fromStdString(mensaje.getText().value()));
    break;

    
  case TipoMensaje::LEFT_ROOM:
    emit salioDeSala(QString::fromStdString(mensaje.getRoomname().value()),
		     QString::fromStdString(mensaje.getUsername().value()));
    break;

    
  case TipoMensaje::DISCONNECTED:
    emit desconectado(QString::fromStdString(mensaje.getUsername().value()));
    break;
    
  default:
    break;
  }
}

void Controlador::manejaResponse(const Mensaje& mensaje){
  OperacionMensaje operacion = mensaje.getOperacion().value();
  ResultadoMensaje resultado = mensaje.getResultado().value();

  switch(operacion){
  case OperacionMensaje::IDENTIFY:
    if(resultado == ResultadoMensaje::SUCCESS)
      emit identificacionExitosa(QString::fromStdString(mensaje.getExtra().value()));
    else
      emit identificacionYaExiste(QString::fromStdString(mensaje.getExtra().value()));
    break;

    
  case OperacionMensaje::TEXT:
    emit usuarioNoEncontrado(QString::fromStdString(mensaje.getExtra().value()));
    break;

    
  case OperacionMensaje::NEW_ROOM:
    if(resultado == ResultadoMensaje::SUCCESS)
      emit salaCreada(QString::fromStdString(mensaje.getExtra().value()));
    else
      emit salaYaExiste(QString::fromStdString(mensaje.getExtra().value()));
    break;

    
  case OperacionMensaje::INVITE:
    if(resultado == ResultadoMensaje::NO_SUCH_ROOM)
      emit salaNoExiste(QString::fromStdString(mensaje.getExtra().value()));
    else
      emit usuarioNoEncontrado(QString::fromStdString(mensaje.getExtra().value()));
    break;

    
  case OperacionMensaje::JOIN_ROOM:
    if(resultado == ResultadoMensaje::SUCCESS)
      emit invitacionAceptada(QString::fromStdString(mensaje.getExtra().value()));
    else if(resultado == ResultadoMensaje::NO_SUCH_ROOM)
      emit salaNoExiste(QString::fromStdString(mensaje.getExtra().value()));
    else
      emit noInvitado(QString::fromStdString(mensaje.getExtra().value()));
    break;

    
  case OperacionMensaje::ROOM_USERS:
  case OperacionMensaje::ROOM_TEXT:
  case OperacionMensaje::LEAVE_ROOM:
    if(resultado == ResultadoMensaje::NO_SUCH_ROOM)
      emit salaNoExiste(QString::fromStdString(mensaje.getExtra().value()));
    else
      emit noUnido(QString::fromStdString(mensaje.getExtra().value()));
    break;

    
  default:
    break;
  }
}
