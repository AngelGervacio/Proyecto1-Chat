#include <iostream>
#include <string>
#include <stdexcept>
#include "Servidor.hpp"

int puerto = -1;

void ayuda(){
  std::string ayuda = "Modo de empleo: ./build/Servidor [OPCIONES...]\n\n"
    "Servidor del chat.\n\n"
    "\u001B[1mOPCIONES:\u001B[0m\n\n" 
    "  -p \u001B[4mpuerto\u001B[0m\n"
    "         Indica el puerto donde estará el servidor.\n\n"
    "  -h \n"
    "         Muestra este mensaje de ayuda.\n\n"
    "\u001B[1mEJEMPLOS:\u001B[0m\n\n"
    "       Las opciones se pueden ingresar en cualquier orden, siempre y cuando se ingresen de forma correcta.\n\n"
    "       ./build/Servidor -p \u001B[4m1234\u001B[0m\n"
    "              Se iniciará el servidor en el puerto 1234.\n\n"
    "       ./build/Servidor -h\n"
    "              Muestra este mensaje de ayuda.\n\n";

  std::cout << ayuda;
}

int configurar(int argc, char* argv[]){
  if(argc < 2){
    std::cout << "Faltan argumentos.\n";
    return 2;
  }
  
  for(int i = 1; i < argc; i++){
    std::string argumento = argv[i];

    if(argumento == "-p"){
      if(puerto != -1){
	std::cout << "El puerto ya fue ingresado.\n";
	return 2;
      }

      if(i+1 >= argc){
	std::cout << "Falta el puerto.\n";
	return 2;
      }

      std::string valor = argv[++i];
      std::size_t pos = 0;
      
      try{
	puerto = std::stoi(valor, &pos);

	if(pos != valor.size()){
	  std::cout << "El puerto debe ser un número entero.\n";
	  return 2;
	}
      }catch(const std::invalid_argument&){
	std::cout << "El puerto ingresado no es válido.\n";
	return 2;
      }catch(const std::out_of_range&){
	std::cout << "El puerto está fuera del rango permitido.\n";
	return 2;
      }

      if(puerto < 1 || puerto > 65535){
	std::cout << "El puerto debe de estar entre 1 y 65535.\n";
	return 2;
      }
      
    }else if(argumento == "-h"){
      ayuda();
      return 1;
      
    }else{
      std::cout << "La bandera " << argumento << " no es valida.\n";
      return 2;
    }
  }

  if(puerto == -1){
    std::cout << "Falta ingresar el puerto.\n";
    return 2;
  }

  return 0;
}

int main(int argc, char* argv[]){
  int resultado = configurar(argc, argv);
  if(resultado == 1)
    return 0;
  else if(resultado == 2){
    std::cout << "Utiliza la bandera -h para ver un mensaje de ayuda.\n";
    return 1;
  }
  
  Servidor servidor(puerto);
  servidor.sirve();

  return 0;
}
