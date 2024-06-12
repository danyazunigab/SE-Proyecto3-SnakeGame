#include <iostream>
#include <chrono>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include "game.h"
#include <unistd.h>

#define PORT 65432

void moveSnake(char direction) {
    // Implementa la lógica para mover la serpiente en el juego aquí
    std::cout << "Moviendo la serpiente en dirección: " << direction << std::endl;
}

int main(int argc, char* argv[]) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Crear descriptor de archivo del socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "Fallo en la creación del socket" << std::endl;
        return -1;
    }
    std::cout << "Socket creado correctamente" << std::endl;

    // Adjuntar socket al puerto
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        std::cerr << "setsockopt falló" << std::endl;
        return -1;
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Enlazar el socket al puerto
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Bind falló" << std::endl;
        return -1;
    }

    if (listen(server_fd, 3) < 0) {
        std::cerr << "Listen falló" << std::endl;
        return -1;
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        std::cerr << "Accept falló" << std::endl;
        return -1;
    }
    std::cout << "Cliente conectado correctamente" << std::endl;
//
//    char buffer[1024] = {0};
//    Game game;
//    game.init("Juego de la Serpiente", 800, 600);
//
//    const std::chrono::milliseconds frame_duration(80);
//
//    while (game.running()) {
//        int valread = read(new_socket, buffer, 1024);
//        if (valread > 0) {
//            std::cout << "Recibido: " << buffer << std::endl;
//            if (buffer[0] == 'R') {
//                std::cout << "Restableciendo juego" << std::endl;
//                // Lógica para restablecer el juego
//                //game.reset();
//            } else {
//                game.handleEvents(buffer[0]);
//            }
//        }
//
//        auto frame_start = std::chrono::steady_clock::now();
//        game.update();
//        game.render();
//
//        auto frame_end = std::chrono::steady_clock::now();
//        auto frame_time = std::chrono::duration_cast<std::chrono::milliseconds>(frame_end - frame_start);
//
//        if (frame_time < frame_duration) {
//            std::this_thread::sleep_for(frame_duration - frame_time);
//        }
//    }
//
//    std::cout << "¡Juego terminado!" << std::endl;
//    game.clean();
    close(new_socket);
    close(server_fd);
    return 0;
}
