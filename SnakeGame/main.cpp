#include <iostream>
#include <chrono>
#include <thread>
#include "game.h"

int main(int argc, char* argv[]) {
    Game game;
    game.init("Snake Game", 800, 600);

    const std::chrono::milliseconds frame_duration(80);

    while (game.running()) {
        auto frame_start = std::chrono::steady_clock::now();

        game.handleEvents();
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
