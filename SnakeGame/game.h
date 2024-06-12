#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <vector>
#include <random>
#include "snake.h"

class Game {
public:
    Game();
    ~Game();
    void init(const char* title, int width, int height);
    void handleEvents(char mov);
    void update();
    void render();
    void clean();
    bool running() { return isRunning; }

private:
    bool isRunning;
    SDL_Window* window;
    SDL_Renderer* renderer;
    Snake* snake;
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<> xDist;
    std::uniform_int_distribution<> yDist;

    struct Fruit {
        SDL_Rect rect;
    };

    std::vector<Fruit> fruits;

    void spawnFruit();
};

#endif // GAME_H
