# MyP Proyecto 1 Chat

### Introducción

Este repositorio contiene la implementación de un chat mediante servidor y cliente, siguiendo un protocolo dado.

El proyecto fue desarrollado en C++, haciendo uso de sockets para permitir la comunicación entre servidor y cliente.

Para poder realizar la comunicación los mensajes son enviados mediante formato JSON como se estableció en el protocolo y haciendo uso de la biblioteca [**JSON for Modern C++**](https://github.com/nlohmann/json).

Por el lado del cliente se hizo uso de el framework [**Qt**](https://www.qt.io/) y su lenguaje de marcado declarativo [**QML**](https://doc.qt.io/qt-6/qmlreference.html).

### Funciones

Este proyecto permite tener un servidor donde se conectaran los clientes, los cuales tiene principalmente la capacidad de:

- Enviar mensajes públicos.
- Enviar mensajes privados a otros usuarios.
- Crear salas e invitar usuarios a estas.
- Enviar mensajes a las salas
- Solicitar listas de usuarios.
- Consultar y cambiar su estatus.

Los clientes se pueden conectar desde distintos dispositivos mientras se encuentren en la misma red y conectandose a la IP y puerto del servidor.

### Requisitos

Las versiones indicadas solo son las utilizadas en el proyecto, no son las versiones minimas necesitadas para ejecutarlo.

Dentro del proyecto ya se encuentran las dependencias de la biblioteca [**JSON for Modern C++**](https://github.com/nlohmann/json) y tambien la biblioteca [**GoogleTest**](https://github.com/google/googletest) para las pruebas unitarias, manteniendo la misma versión de estas.

#### Meson y Ninja

Para poder compilar y ejecutar las pruebas unitarias se hace uso de el sistema de construcción [**Meson (1.4.1)**](https://mesonbuild.com/), este hace uso del sistema de compilación [**Ninja (1.12.1)**](https://ninja-build.org/), de manera que se puede instalar de la siguente manera:

- En Debian / Ubuntu / Linux Mint:

```
sudo apt install meson ninja-build
```

- En Fedora / Red Hat:

```
sudo dnf install meson ninja-build
```

#### g++

Como el proyecto esta escrito en C++ hacemos uso del compilador **g++ (14.2.1)**, el cual se puede instalar mediante el paquete *build-essential*:

- En Debian / Ubuntu / Linux Mint:
 
```
sudo apt install build-essential
```

y de manera individual como:

```
sudo apt install g++
```

- En Fedora / Red Hat:

```
sudo dnf install gcc-c++
```

#### Qt

Para la creación de la interfaz grafica de usuario se usa [**Qt (6.8.2)**](https://www.qt.io/), es recomendable instalar el IDE [**QtCreator (15.0.0)**](https://www.qt.io/development/tools/qt-creator-ide) junto con herramientas adicionales:

- En Debian/ Ubuntu / Linux Mint:

```
sudo apt install qt6-base-dev qt6-declarative-dev qtcreator
```

Pueden hacer falta algunos elementos que impidan la ejecución correcta del cliente, estos se pueden instalar como:

```
sudo apt install qml6-module-qtquick 
sudo apt install qml6-module-qtquick-controls 
sudo apt install qml6-module-qtqml-workerscript
sudo apt install qml6-module-qtquick-layouts
sudo apt install qml6-module-qtquick-templates
```

Pueden hacer falta otros, sin embargo, su instalación es similar.

- En Fedora / Red Hat:

```
sudo dnf install qt6-qtbase-devel qt6-qtdeclarative-devel qtcreator
```

#### Doxygen

Por ultimo para poder generar la documentación del proyecto, se hizo uso de [**Doxygen (1.10.0)**](https://www.doxygen.nl/), el cual se puede instalar como:

- En Debian / Ubuntu / Linux Mint:

```
sudo apt install doxygen
```

- En Fedora / Red Hat:

```
sudo dnf install doxygen
```

### Compilación y pruebas unitarias

Primero es necesario clonar el repositorio.

```
git clone https://github.com/AngelGervacio/Proyecto1-Chat.git
```

Dentro de la carpeta del repositorio configuramos el proyecto con **Meson**.

```
meson setup build
```

Una vez configurado, podemos compilar el proyecto.

```
meson compile -C build
```

Despues de que se compile, podemos ejecutar las pruebas unitarias.

```
meson test -C build
```

### Ejecución del servidor y cliente

Una vez compilado el proyecto sin errores, se generan los ejecutables de el servidor y el cliente.


#### Servidor

El servidor recibe dos banderas:

- **-p** *puerto*
- **-h**

La bandera **-p** va junto con el puerto donde se iniciara el servidor, mientras que la bandera **-h** muestra un mensaje de ayuda.

De manera que para ejecutarlo hacemos:

```
./build/Servidor -p 1234
```

Y también

```
./build/Servidor -h
```

#### Cliente

El cliente recibe tres banderas:

- **-i** *IP*
- **-p** *puerto*
- **-h**

La bandera **-i** necesita la IP del servidor, con **-p** seguido del puerto especificamos el puerto donde esta el servidor y con **-h** se muestra un mensaje de ayuda.

Y para ejecutarlo:

```
./build/ClienteGUI -i 127.0.0.1 -p 1234
```

Y también

```
./build/ClienteGUI -h
```

### Documentación

Para poder generar la documentación del proyecto hacemos uso de **Doxygen** y ejecutamos:

```
doxygen Doxyfile
```

Y para abrirlo desde la terminal podemos usar la herramienta **xdg-open** tal que:

```
xdg-open docs/html/index.html
```

La pagina generada contendra la información de las clases asi como sus metodos implementados.

### Integración Continua

El proyecto hace uso de GitHub Actions pare configurar el proyecto, compilarlo, ejecutar pruebas unitarias y generar documentación cada vez que se realiza un **push** o un **pull request**.