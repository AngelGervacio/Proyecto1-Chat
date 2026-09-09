/**
 * @file TestUsuario.cpp
 * @brief Pruebas unitarias para la clase Usuario.
 */

#include <gtest/gtest.h>
#include <stdexcept>
#include "Usuario.hpp"
#include "EstatusUsuario.hpp"

/**
 * @brief Prueba del constructor de la clase Usuario.
 * Comprueba que se asigne correctamente el nombre,
 * que el estatus inicial sea ACTIVE y que su longitud
 * no tenga una longitud mayor a la permitida.
 */
TEST(UsuarioTest, ConstructorUsuario){
  Usuario usuario("Username");

  EXPECT_EQ(usuario.getNombre(), "Username");
  EXPECT_EQ(usuario.getEstatus(), EstatusUsuario::ACTIVE);
  EXPECT_THROW(Usuario(""), std::invalid_argument);
  EXPECT_THROW(Usuario("NoValido123"), std::invalid_argument);
  
}

/**
 * @brief Prueba del metodo getNombre() de la clase Usuario.
 * Comprueba que se asigne correctamente el nombre al usuario.
 */
TEST(UsuarioTest, ObtenerNombre){
  Usuario usuario1("Alice");
  Usuario usuario2("Bob");

  EXPECT_EQ(usuario1.getNombre(), "Alice");
  EXPECT_EQ(usuario2.getNombre(), "Bob");
}

/**
 * @brief Prueba del metodo getEstatus() de la clase Usuario.
 * Comprueba que el estatus inicial sea ACTIVE.
 */
TEST(UsuarioTest, ObtenerEstatus){
  Usuario usuario1("Alice");
  Usuario usuario2("Bob");

  EXPECT_EQ(usuario1.getEstatus(), EstatusUsuario::ACTIVE);
  EXPECT_EQ(usuario2.getEstatus(), EstatusUsuario::ACTIVE);
}

/**
 * @brief Prueba del metodo setEstatus() de la clase Usuario.
 * Comprueba que se pueda actualizar correctamente el estatus
 * del usuario a ACTIVE, AWAY y BUSY.
 */
TEST(UsuarioTest, ActualizaEstatus){
  Usuario usuario("Username");

  EXPECT_EQ(usuario.getEstatus(), EstatusUsuario::ACTIVE);
  
  usuario.setEstatus(EstatusUsuario::AWAY);
  EXPECT_EQ(usuario.getEstatus(), EstatusUsuario::AWAY);
  
  usuario.setEstatus(EstatusUsuario::BUSY);
  EXPECT_EQ(usuario.getEstatus(), EstatusUsuario::BUSY);
  
  usuario.setEstatus(EstatusUsuario::ACTIVE);
  EXPECT_EQ(usuario.getEstatus(), EstatusUsuario::ACTIVE);
}

/**
 * @brief Prueba el metodo operator==() de la clase Usuario.
 * Comprueba que se comparen dos Usuarios correctamente.
 */
TEST(UsuarioTest, ComparaUsuarios){
  Usuario usuario("Alice");
  Usuario otroUsuario("Alice");

  EXPECT_TRUE(usuario == otroUsuario);

  otroUsuario.setEstatus(EstatusUsuario::BUSY);

  EXPECT_FALSE(usuario == otroUsuario);

  Usuario nuevoUsuario("Bob");

  EXPECT_FALSE(usuario == nuevoUsuario);
}
