#include "snake.h"
#include <omp.h>

Snake::Snake() : dirX(20), dirY(0) {
    SDL_Rect head = { 100, 100, 20, 20 };
    body.push_back(head);
}

Snake::~Snake() {}

void Snake::handleInput(SDL_Event& event, char mov) {
    switch (mov) {
        case 'w':
            if (dirY == 0) { dirX = 0; dirY = -20; }
            break;
        case 's':
            if (dirY == 0) { dirX = 0; dirY = 20; }
            break;
        case 'a':
            if (dirX == 0) { dirX = -20; dirY = 0; }
            break;
        case 'd':
            if (dirX == 0) { dirX = 20; dirY = 0; }
            break;
        }
    /*if (event.type == SDL_KEYDOWN) {
        switch (mov) {
        case 'w':
            if (dirY == 0) { dirX = 0; dirY = -20; }
            break;
        case 's':
            if (dirY == 0) { dirX = 0; dirY = 20; }
            break;
        case 'a':
            if (dirX == 0) { dirX = -20; dirY = 0; }
            break;
        case 'd':
            if (dirX == 0) { dirX = 20; dirY = 0; }
            break;
        }
    }*/
}

void Snake::update() {
    #pragma omp parallel for
    for (int i = body.size() - 1; i > 0; --i) {
        body[i] = body[i - 1];
    }
    body[0].x += dirX;
    body[0].y += dirY;

    // Wrap the snake around the screen edges
    if (body[0].x < 0) body[0].x = 780;
    if (body[0].x >= 800) body[0].x = 0;
    if (body[0].y < 0) body[0].y = 580;
    if (body[0].y >= 600) body[0].y = 0;
}


void Snake::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (const auto& segment : body) {
        SDL_RenderFillRect(renderer, &segment);
    }
}

void Snake::grow() {
    SDL_Rect tail = body.back();
    body.push_back({ tail.x, tail.y, tail.w, tail.h });
}

SDL_Rect& Snake::getHead() {
    return body[0];
}

std::vector<SDL_Rect>& Snake::getBody() {
    return body;
}
