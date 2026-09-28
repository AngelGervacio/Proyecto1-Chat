/**
 * @file Util.hpp
 * @brief Contiene métodos usados en más de un Test.
 */

#ifndef UTIL_HPP
#define UTIL_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <chrono>

/**
 * @brief Regresa un puerto disponible.
 * @return El puerto disponible.
 */
inline int puertoAleatorio(){
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

#endif
