/**
 * @file TestUsuario.cpp
 * @brief Pruebas unitarias para la clase Usuario.
 */

#include <gtest/gtest.h>
#include <memory>
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

  std::unique_ptr<Usuario> usuario;
  usuario = std::make_unique<Usuario>(std::make_unique<std::string>("Username"));

  EXPECT_EQ(usuario->getNombre(), "Username");

  EXPECT_EQ(usuario->getEstatus(), EstatusUsuario::ACTIVE);

  EXPECT_THROW(std::make_unique<Usuario>(std::make_unique<std::string>()), std::invalid_argument);

  EXPECT_THROW(std::make_unique<Usuario>(std::make_unique<std::string>("")), std::invalid_argument);
  
  EXPECT_THROW(std::make_unique<Usuario>(std::make_unique<std::string>("NoValido123")), std::invalid_argument);
  
}

/**
 * @brief Prueba del metodo getNombre() de la clase Usuario.
 * Comprueba que se asigne correctamente el nombre al usuario.
 */
TEST(UsuarioTest, ObtenerNombre){

  std::unique_ptr<Usuario> usuario1;
  usuario1 = std::make_unique<Usuario>(std::make_unique<std::string>("Jose"));

  std::unique_ptr<Usuario> usuario2;
  usuario2 = std::make_unique<Usuario>(std::make_unique<std::string>("Mariana"));

  EXPECT_EQ(usuario1->getNombre(), "Jose");
  EXPECT_EQ(usuario2->getNombre(), "Mariana");
}

/**
 * @brief Prueba del metodo getEstatus() de la clase Usuario.
 * Comprueba que el estatus inicial sea ACTIVE.
 */
TEST(UsuarioTest, ObtenerEstatus){

  std::unique_ptr<Usuario> usuario1;
  usuario1 = std::make_unique<Usuario>(std::make_unique<std::string>("Jose"));

  std::unique_ptr<Usuario> usuario2;
  usuario2 = std::make_unique<Usuario>(std::make_unique<std::string>("Mariana"));

  EXPECT_EQ(usuario1->getEstatus(), EstatusUsuario::ACTIVE);
  EXPECT_EQ(usuario2->getEstatus(), EstatusUsuario::ACTIVE);
}

/**
 * @brief Prueba del metodo setEstatus() de la clase Usuario.
 * Comprueba que se pueda actualizar correctamente el estatus
 * del usuario a ACTIVE, AWAY y BUSY.
 */
TEST(UsuarioTest, ActualizaEstatus){

  std::unique_ptr<Usuario> usuario;
  usuario = std::make_unique<Usuario>(std::make_unique<std::string>("Username"));

  EXPECT_EQ(usuario->getEstatus(), EstatusUsuario::ACTIVE);

  usuario->setEstatus(std::make_unique<EstatusUsuario>(EstatusUsuario::AWAY));

  EXPECT_EQ(usuario->getEstatus(), EstatusUsuario::AWAY);

  usuario->setEstatus(std::make_unique<EstatusUsuario>(EstatusUsuario::BUSY));

  EXPECT_EQ(usuario->getEstatus(), EstatusUsuario::BUSY);

  usuario->setEstatus(std::make_unique<EstatusUsuario>(EstatusUsuario::ACTIVE));

  EXPECT_EQ(usuario->getEstatus(), EstatusUsuario::ACTIVE);
}
