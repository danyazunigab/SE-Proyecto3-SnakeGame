#include "game.h"
#include <iostream>
#include <omp.h>
#include<chrono>

Game::Game() : isRunning(false), window(nullptr), renderer(nullptr), snake(nullptr), gen(rd()), xDist(0, 39), yDist(0, 29) {}

Game::~Game() {
    delete snake;
}

void Game::init(const char* title, int width, int height) {
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
        if (!window) {
            std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
            return;
        }
        renderer = SDL_CreateRenderer(window, -1, 0);
        if (!renderer) {
            std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
            return;
        }
        isRunning = true;
    } else {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return;
    }
    snake = new Snake();
    spawnFruit(); // Generar la primera fruta al inicializar el juego
}

void Game::handleEvents(char mov) {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
    case SDL_QUIT:
        isRunning = false;
        break;
    default:
        snake->handleInput(event,mov);
        break;
    }
}

void Game::update() {
    snake->update();

    // Detectar si la serpiente ha colisionado consigo misma
    for (size_t i = 1; i < snake->getBody().size(); ++i) {
        if (SDL_HasIntersection(&snake->getHead(), &snake->getBody()[i])) {
            isRunning = false;
            return;
        }
    }

    // Detectar si la serpiente ha colisionado con los bordes de la pantalla
    if (snake->getHead().x < 0 || snake->getHead().x >= 800 || snake->getHead().y < 0 || snake->getHead().y >= 600) {
        isRunning = false;
        return;
    }

    // Detectar si la serpiente ha comido una fruta
    for (size_t i = 0; i < fruits.size(); ++i) {
        if (SDL_HasIntersection(&snake->getHead(), &fruits[i].rect)) {
            fruits.erase(fruits.begin() + i);
            snake->grow();
            spawnFruit();
            break;
        }
    }
}

void Game::render() {
    static int frame_count = 0;
    static auto last_time = std::chrono::steady_clock::now();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    snake->render(renderer);

    // Renderizar las frutas
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Color rojo para las frutas
    #pragma omp parallel for
    for (int i = 0; i < fruits.size(); ++i) {
        SDL_RenderFillRect(renderer, &fruits[i].rect);
    }

    SDL_RenderPresent(renderer);
    frame_count++;
    auto current_time = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(current_time - last_time).count();

    if (duration >= 1) {
        std::cout << "FPS: " << frame_count / duration << std::endl;
        frame_count = 0;
        last_time = current_time;
    }
}


void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

void Game::spawnFruit() {
    Fruit newFruit;
    newFruit.rect.x = xDist(gen) * 20;
    newFruit.rect.y = yDist(gen) * 20;
    newFruit.rect.w = 20;
    newFruit.rect.h = 20;
    fruits.push_back(newFruit);
}
