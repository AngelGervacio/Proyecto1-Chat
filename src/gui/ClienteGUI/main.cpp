#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <string>
#include <QUrl>
#include <arpa/inet.h>
#include <iostream>
#include "../../Controlador.hpp"

std::string ipServidor = "";
int puerto = -1;

void ayuda(){
  std::string ayuda = "Modo de empleo: ./build/ClienteGUI [OPCIONES...]\n\n"
    "Cliente del chat.\n\n"
    "\u001B[1mOPCIONES:\u001B[0m\n\n" 
    "  -p \u001B[4mpuerto\u001B[0m\n"
    "         Indica el puerto donde se conectará el cliente.\n\n"
    "  -i \u001B[4mIP\u001B[0m\n"
    "         Indica la dirección IP a donde se conectará el cliente.\n\n"
    "  -h \n"
    "         Muestra este mensaje de ayuda.\n\n"
    "\u001B[1mEJEMPLOS:\u001B[0m\n\n"
    "       Las opciones se pueden ingresar en cualquier orden, siempre y cuando se ingresen de forma correcta.\n\n"
    "       ./build/ClienteGUI -p \u001B[4m1234\u001B[0m -i \u001B[4m127.0.0.1\u001B[0m\n"
    "              El cliente se conectará a la IP 127.0.0.1 en el puerto 1234.\n\n"
    "       ./build/ClienteGUI -h\n"
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
      
    }else if(argumento == "-i"){
      if(!ipServidor.empty()){
	std::cout << "La dirección IP ya fue ingresada.\n";
	return 2;
      }

      if(i+1 >= argc){
	std::cout << "Falta ingresar la dirección IP.\n";
	return 2;
      }

      ipServidor = argv[++i];
      
    }else if(argumento == "-h"){
      ayuda();
      return 1;
      
    }else{
      std::cout << "La bandera " << argumento << " no es válida.\n";
      return 2;
    }
  }

  if(ipServidor.empty()){
    std::cout << "Falta ingresar la dirección IP.\n";
    return 2;
  }

  struct in_addr direccion;

  if(inet_pton(AF_INET, ipServidor.c_str(), &direccion) != 1){
    std::cout << "La dirección IP ingresada no es válida.\n";
    return 2;
  }

  if(puerto == -1){
    std::cout << "Falta ingresar el puerto.\n";
    return 2;
  }

  return 0;
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    int resultado = configurar(argc, argv);

    if(resultado == 1)
      return 0;
    else if(resultado == 2){
      std::cout << "Utiliza la bandera -h para ver un mensaje de ayuda.\n";
      return 1;
    }
    
    QQmlApplicationEngine engine;

    Controlador controlador(ipServidor, puerto);
    engine.rootContext()->setContextProperty("controlador", &controlador);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(QUrl(QStringLiteral("src/gui/ClienteGUI/Main.qml")));

    return app.exec();
}
