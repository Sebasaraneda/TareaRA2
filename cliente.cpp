#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <IP> <puerto>\n";
        return 1;
    }

    std::string ip = argv[1];
    int puerto = std::stoi(argv[2]);

    int clienteSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clienteSocket == -1) {
        std::cerr << "No se pudo crear el socket del cliente.\n";
        return 1;
    }

    sockaddr_in servidorAddr{};
    servidorAddr.sin_family = AF_INET;
    servidorAddr.sin_port = htons(puerto);
    if (inet_pton(AF_INET, ip.c_str(), &servidorAddr.sin_addr) <= 0) {
        std::cerr << "Dirección IP inválida.\n";
        return 1;
    }

    if (connect(clienteSocket, (struct sockaddr*)&servidorAddr, sizeof(servidorAddr)) < 0) {
        std::cerr << "No se pudo conectar al servidor.\n";
        return 1;
    }

    std::cout << "Conectado con éxito al servidor.\n";

    // Bucle de juego
    char buffer[1024];
    while (true) {
        // Recibir y mostrar el estado del tablero
        ssize_t bytesRecibidos = recv(clienteSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesRecibidos <= 0) {
            std::cerr << "Error al recibir datos del servidor o el servidor cerró la conexión.\n";
            break;
        }
        buffer[bytesRecibidos] = '\0';
        std::cout << "Estado del tablero:\n" << buffer << '\n';

        // Si es el turno del usuario, solicitar una entrada de columna y enviarla al servidor
        std::cout << "Ingresa el número de columna donde quieres dejar caer tu ficha: ";
        int columna;
        std::cin >> columna;
        std::string mensaje = std::to_string(columna) + '\n';
        send(clienteSocket, mensaje.c_str(), mensaje.size(), 0);
        std::cout << "Mensaje enviado al servidor: " << mensaje;

        // Recibir el resultado del movimiento del servidor y mostrarlo al usuario
        bytesRecibidos = recv(clienteSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesRecibidos <= 0) {
            std::cerr << "Error al recibir datos del servidor o el servidor cerró la conexión.\n";
            break;
        }
        buffer[bytesRecibidos] = '\0';
        std::cout << "Respuesta del servidor: " << buffer << '\n';
    }

    close(clienteSocket);

    return 0;
}