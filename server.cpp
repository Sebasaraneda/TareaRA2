#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>

// Representación del tablero de juego
std::vector<std::vector<char>> tablero(6, std::vector<char>(7, ' '));

// Verificar si un jugador ha ganado
bool verificarGanador(int fila, int columna) {
    const int direcciones[4][2] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};
    char jugador = tablero[fila][columna];

    std::cout << "Verificando ganador para el jugador " << jugador << " en la posición (" << fila << ", " << columna << ")\n";

    for (int d = 0; d < 4; ++d) {
        int contador = 0;
        for (int i = -3; i <= 3; ++i) {
            int x = fila + i * direcciones[d][0];
            int y = columna + i * direcciones[d][1];

            if (x >= 0 && x < 6 && y >= 0 && y < 7 && tablero[x][y] == jugador) {
                if (++contador == 4) {
                    std::cout << "El jugador " << jugador << " ha ganado.\n";
                    return true;
                }
            } else {
                contador = 0;
            }
        }
    }

    std::cout << "No se encontró un ganador para el jugador " << jugador << " en la posición (" << fila << ", " << columna << ")\n";
    return false;
}

// Movimientos
bool hacerMovimiento(int columna, char jugador) {
    for (int i = 5; i >= 0; --i) {
        if (tablero[i][columna] == ' ') {
            tablero[i][columna] = jugador;
            return verificarGanador(i, columna);
        }
    }
    return false;
}

void manejarCliente(int clienteSocket) {
    char buffer[1024];
    while (true) {
        // Enviar el estado inicial del tablero al cliente
        for (int i = 0; i < 6; ++i) {
            for (int j = 0; j < 7; ++j) {
                tablero[i][j] = ' ';
            }
        }
        std::string estadoTablero;
        for (int i = 0; i < 6; ++i) {
            for (int j = 0; j < 7; ++j) {
                estadoTablero += tablero[i][j];
            }
            estadoTablero += '\n';
        }
        send(clienteSocket, estadoTablero.c_str(), estadoTablero.size(), 0);

        // Recibir el movimiento del cliente
        ssize_t bytesRecibidos = recv(clienteSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesRecibidos <= 0) {
            std::cerr << "Error al recibir datos del cliente o el cliente cerró la conexión.\n";
            break;
        }
        buffer[bytesRecibidos] = '\0';

        // Convertir el movimiento a un número de columna
        int columna = std::stoi(buffer);

        // Hacer el movimiento y verificar si el cliente ha ganado
        bool clienteGano = hacerMovimiento(columna, 'C');
        if (clienteGano) {
            // Enviar al cliente mensaje de que ha ganado
            std::string mensaje = "Has ganado!\n";
            send(clienteSocket, mensaje.c_str(), mensaje.size(), 0);
            break;
        }

        // Hacer un movimiento para el servidor
        std::srand(std::time(nullptr)); // Semilla para la generación de números aleatorios
        int columnaServidor = std::rand() % 7; // Generar un número de columna al azar entre 0 y 6
        bool servidorGano = hacerMovimiento(columnaServidor, 'S');

        // Enviar el nuevo estado del tablero al cliente
        estadoTablero.clear();
        for (int i = 0; i < 6; ++i) {
            for (int j = 0; j < 7; ++j) {
                estadoTablero += tablero[i][j];
            }
            estadoTablero += '\n';
        }
        send(clienteSocket, estadoTablero.c_str(), estadoTablero.size(), 0);

        // Si el servidor ha ganado, terminar el juego
        if (servidorGano) {
            // Enviar al cliente mensaje de que el servidor ha ganado
            std::string mensaje = "El servidor ha ganado!\n";
            send(clienteSocket, mensaje.c_str(), mensaje.size(), 0);
            break;
        }

        // Enviar un mensaje al cliente para indicar que es su turno para mover
        std::string mensaje = "Es tu turno para mover.\n";
        send(clienteSocket, mensaje.c_str(), mensaje.size(), 0);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <puerto>\n";
        return 1;
    }

    int puerto = std::stoi(argv[1]);

    int servidorSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (servidorSocket == -1) {
        std::cerr << "No se pudo crear el socket del servidor.\n";
        return 1;
    }

    sockaddr_in servidorAddr{};
    servidorAddr.sin_family = AF_INET;
    servidorAddr.sin_port = htons(puerto);
    servidorAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(servidorSocket, (struct sockaddr*)&servidorAddr, sizeof(servidorAddr)) == -1) {
        std::cerr << "No se pudo enlazar el socket del servidor.\n";
        return 1;
    }

    if (listen(servidorSocket, SOMAXCONN) == -1) {
        std::cerr << "No se pudo escuchar en el socket del servidor.\n";
        return 1;
    }

    while (true) {
        sockaddr_in clienteAddr{};
        socklen_t clienteLen = sizeof(clienteAddr);
        int clienteSocket = accept(servidorSocket, (struct sockaddr*)&clienteAddr, &clienteLen);

        if (clienteSocket == -1) {
            std::cerr << "No se pudo aceptar la conexión del cliente.\n";
            continue;
        }

        std::thread t(manejarCliente, clienteSocket);
        t.detach();
    }

    close(servidorSocket);

    return 0;
}