#ifndef SNAKE_H
#define SNAKE_H

#include "SDL2/SDL.h"
#include <vector>

class Snake {
public:
    Snake();
    ~Snake();

    void handleInput(SDL_Event& event, char mov);
    void update();
    void render(SDL_Renderer* renderer);
    void grow();
    SDL_Rect& getHead();
    std::vector<SDL_Rect>& getBody();

private:
    std::vector<SDL_Rect> body;
    int dirX, dirY;
};

#endif
