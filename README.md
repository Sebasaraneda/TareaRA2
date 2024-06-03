# Tarea 1 CDR Cuatro en linea 
## Descripción
Implementar el Juego Cuatro en Línea mediante el modelo Cliente/Servidor. Para ello, debe crear dos programas de los cuales uno funcionará como el Servidor de juegos (con peticiones concurrentes) y el otro será el Cliente que se conectará al servidor para jugar.

## Objetivos 

Utilizar diversos mecanismos relacionados con los procesos, incluyendo la creación y finalización de procesos, y los mecanismos de comunicación.
Definir e implementar un protocolo para la capa de aplicación.
Utilizar el protocolo orientado a la conexión TCP de la capa de transporte.

## Pre-requisitos
Antes de ejecutar los código hay que tener las dependencias necesarias instaladas.

_Con este comando se instala el compilador de C++_
```bash
   sudo apt-get install g++
   ```
_Y este comando para instalar la herramienta **make**_
```bash
   sudo apt-get install make
   ```

## Compilación y ejecución
_Ubicarse dentro de la carpeta que contiene los archivos del programa y ejecutar el comando **make** para compilar._
```bash
   make
   ```
_Posteriormente, ejecutar el programa del server._
```bash
   ./server <puerto>
   ```
_Despues, ejecutar el programa del cliente ingresando la ip y el puerto del servidor._
```bash
   ./cliente <ip> <puerto>
   ```
## Integrantes:
Sebastián Araneda Y 
Alexandra Sepúlveda.
