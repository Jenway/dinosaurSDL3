#define SDL_MAIN_HANDLED
#include "../include/game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_log.h>
#include <stdio.h>

int main(int argc, char* argv[])
{

    // initialize SDL and SDL_image
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return 1;
    }
    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != (IMG_INIT_PNG | IMG_INIT_JPG)) {
        SDL_Log("Failed to initialize SDL_image");
        return 1;
    }

    // create window
    SDL_Window* window = SDL_CreateWindow("Chrome Dino", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    if (!window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return 1;
    }

    // create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return 1;
    }
    // load resources
    if (!Game_Load(renderer)) {
        SDL_Log("Failed to load game");
        return 1;
    }

    // 运行游戏循环
    bool quit = false;
    while (!quit) {
        Game_Init();
        SDL_Log("Game Initialized");
        Game_loop(renderer);
        SDL_Log("Game Over");
        quit = true;
        // if (quit) {
        //     break;
        // } else {
        //     Game_Reset();
        //     SDL_Log("Game Reset");
        // }
    }

    // 清理资源
    Game_Destroy();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
