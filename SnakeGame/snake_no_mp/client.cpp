#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <termios.h> // Incluir la cabecera para capturar la entrada del teclado

#define PORT 65432
#define SERVER_IP "127.0.0.1"

int main() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection Failed" << std::endl;
        return -1;
    }

    // Configurar el modo de terminal para capturar la entrada del teclado sin necesidad de presionar Enter
    struct termios oldSettings, newSettings;
    tcgetattr(STDIN_FILENO, &oldSettings);
    newSettings = oldSettings;
    newSettings.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newSettings);

    std::cout << "Presiona las teclas W (arriba), S (abajo), A (izquierda) o D (derecha) para mover la serpiente. Presiona Q para salir." << std::endl;

    // Leer la entrada del usuario y enviarla al servidor
    char direction;
    while (true) {
        direction = getchar();
        if (direction == 'q' || direction == 'Q') {
            break;
        }
        send(sock, &direction, sizeof(direction), 0);
    }

    // Restaurar la configuración original de la terminal
    tcsetattr(STDIN_FILENO, TCSANOW, &oldSettings);

    return 0;
}
