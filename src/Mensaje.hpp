/**
 * @file Mensaje.hpp
 * @brief Header de la clase Mensaje.
 */

#ifndef MENSAJE_HPP
#define MENSAJE_HPP

#include <string>
#include <optional>
#include <list>
#include "Usuario.hpp"
#include "EstatusUsuario.hpp"
#include "TipoMensaje.hpp"
#include "OperacionMensaje.hpp"
#include "ResultadoMensaje.hpp"

/**
 * @class Mensaje
 * @brief Representa un Mensaje.
 */
class Mensaje{

private:
  TipoMensaje tipo; /*!< Tipo de Mensaje */
  std::optional<OperacionMensaje> operacion; /*!< Operación del Mensaje */
  std::optional<ResultadoMensaje> resultado; /*!< Resultado de la operación */
  std::optional<std::string> username; /*!< Nombre de Usuario */
  std::optional<std::string> extra; /*!< Contenido extra */
  std::optional<EstatusUsuario> estatus; /*!< Estatus de Usuario */
  std::optional<std::list<Usuario>> users; /*!< Lista de usuarios */
  std::optional<std::string> text; /*!< Texto enviado */
  std::optional<std::string> roomname; /*!< Nombre de la sala */
  std::optional<std::list<std::string>> usernames; /*!< Nombres de usuario */

public:

  /**
   * @class Builder
   * @brief Patron de diseño Builder.
   */
  class Builder{
    
  private:
    std::optional<TipoMensaje> tipo; /*!< Tipo de Mensaje */
    std::optional<OperacionMensaje> operacion; /*!< Operación del Mensaje */
    std::optional<ResultadoMensaje> resultado; /*!< Resultado de la operación */
    std::optional<std::string> username; /*!< Nombre de Usuario */
    std::optional<std::string> extra; /*!< Contenido extra */
    std::optional<EstatusUsuario> estatus; /*!< Estatus de Usuario */
    std::optional<std::list<Usuario>> users; /*!< Lista de usuarios */
    std::optional<std::string> text; /*!< Texto enviado */
    std::optional<std::string> roomname; /*!< Nombre de la sala */
    std::optional<std::list<std::string>> usernames; /*!< Nombres de usuario */

  public:

    /**
     * @brief Agrega el tipo de Mensaje.
     * @param tipo El tipo de Mensaje.
     */
    Builder& setTipo(TipoMensaje tipo);
    
    /**
     * @brief Agrega la operación de Mensaje.
     * @param operacion La operación del Mensaje.
     */
    Builder& setOperacion(OperacionMensaje operacion);
    
    /**
     * @brief Agrega el resultado de la operación.
     * @param resultado El resultado de la operación.
     */
    Builder& setResultado(ResultadoMensaje resultado);
    
    /**
     * @brief Agrega el nombre de usuario en el Mensaje.
     * @param username El nombre de usuario.
     */
    Builder& setUsername(std::string username);
    
    /**
     * @brief Agrega el contenido extra del Mensaje.
     * @param extra El contenido extra.
     */
    Builder& setExtra(std::string extra);
    
    /**
     * @brief Agrega el estatus del usuario del Mensaje.
     * @param estatus El estatus del usuario.
     */
    Builder& setEstatus(EstatusUsuario estatus);
    
    /**
     * @brief Agrega la lista de usuarios al Mensaje.
     * @param users La lista de usuarios.
     */
    Builder& setUsers(std::list<Usuario> users);
    
    /**
     * @brief Agrega el texto enviado al Mensaje
     * @param text El texto enviado.
     */
    Builder& setText(std::string text);
    
    /**
     * @brief Agrega el nombre de la sala al Mensaje.
     * @param roomname El nombre de la sala.
     */
    Builder& setRoomname(std::string roomname);
    
    /**
     * @brief Agrega la lista de nombres de usuario al Mensaje.
     * @param usernames La lista de nombres de usuario.
     */
    Builder& setUsernames(std::list<std::string> usernames);
    
    /**
     * @brief Construye el Mensaje.
     */
    Mensaje build();
  };
  
  /**
   * @brief Constructor de la clase Mensaje.
   * @param tipo El tipo del Mensaje.
   * @param operacion La operación del Mensaje.
   * @param resultado El resultado del Mensaje.
   * @param username El nombre de usuario.
   * @param extra El contenido extra.
   * @param estatus El estatus del usuario.
   * @param users La lista de usuarios.
   * @param text El texto enviado.
   * @param roomname El nombre de la sala.
   * @param usernames La lista de nombres de usuario.
   */
  Mensaje(TipoMensaje tipo,
	  std::optional<OperacionMensaje> operacion,
	  std::optional<ResultadoMensaje> resultado,
	  std::optional<std::string> username,
	  std::optional<std::string> extra,
	  std::optional<EstatusUsuario> estatus,
	  std::optional<std::list<Usuario>> users,
	  std::optional<std::string> text,
	  std::optional<std::string> roomname,
	  std::optional<std::list<std::string>> usernames);
  
  /**
   * @brief Regresa el tipo de Mensaje
   * @return El tipo de Mensaje.
   */
  TipoMensaje getTipo() const;

  /**
   * @brief Regresa la operación del Mensaje.
   * @return La operación del Mensaje
   */
  std::optional<OperacionMensaje> getOperacion() const;

  /**
   * @brief Regresa el resultado de la operación.
   * @return El resultado de la operación.
   */
  std::optional<ResultadoMensaje> getResultado() const;

  /**
   * @brief Regresa el nombre de usuario.
   * @return El nombre de usuario.
   */
  std::optional<std::string> getUsername() const;

  /**
   * @brief Regresa el contenido extra.
   * @return El contenido extra.
   */
  std::optional<std::string> getExtra() const;

  /**
   * @brief Regresa el estatus del usuario.
   * @return El estatus del usuario.
   */
  std::optional<EstatusUsuario> getEstatus() const;

  /**
   * @brief Regresa la lista de usuarios.
   * @return La lista de usuarios.
   */
  std::optional<std::list<Usuario>> getUsers() const;

  /**
   * @brief Regresa el texto enviado.
   * @return El texto enviado.
   */
  std::optional<std::string> getText() const;

  /**
   * @brief Regresa el nombre de la sala.
   * @return El nombre de la sala.
   */
  std::optional<std::string> getRoomname() const;

  /**
   * @brief Regresa la lista de nombres de usuarios.
   * @return La lista de nombres de usuarios.
   */
  std::optional<std::list<std::string>> getUsernames() const;
  
};

#endif
