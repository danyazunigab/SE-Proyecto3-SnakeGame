#include <iostream>
#include <chrono>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include "game.h"
#include <unistd.h>
#include <fcntl.h>

#define PORT 65432

void moveSnake(char direction) {
    // Aquí puedes implementar la lógica para mover la serpiente en el juego
    std::cout << "Moving snake in direction: " << direction << std::endl;
}

int main(int argc, char* argv[]) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Crear socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "socket failed" << std::endl;
        return -1;
    }

    // Adjuntar socket al puerto
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        std::cerr << "setsockopt failed" << std::endl;
        return -1;
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Adjuntar socket al puerto
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "bind failed" << std::endl;
        return -1;
    }

    if (listen(server_fd, 3) < 0) {
        std::cerr << "listen failed" << std::endl;
        return -1;
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        std::cerr << "accept failed" << std::endl;
        return -1;
    }
    int flags = fcntl(new_socket, F_GETFL, 0);
    fcntl(new_socket, F_SETFL, flags | O_NONBLOCK);

    char buffer[1024] = {0};
    int valread;
    Game game;
    game.init("Snake Game", 800, 600);

    const std::chrono::milliseconds frame_duration(100);

    while (game.running()) {
         valread = read(new_socket, buffer, 1024);
        if (valread > 0) {
            game.handleEvents(buffer[0]);
        }
        auto frame_start = std::chrono::steady_clock::now();
        game.update();
        game.render();

        auto frame_end = std::chrono::steady_clock::now();
        auto frame_time = std::chrono::duration_cast<std::chrono::milliseconds>(frame_end - frame_start);

        if (frame_time < frame_duration) {
            std::this_thread::sleep_for(frame_duration - frame_time);
        }
    }

    std::cout << "Game Over!" << std::endl;
    game.clean();
    return 0;
}
