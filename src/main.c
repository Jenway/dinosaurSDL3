#define SDL_MAIN_HANDLED
#include "../include/game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_log.h>
#include <stdio.h>

int main(int argc, char* argv[])
{

    // 初始化 SDL2 库
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return 1;
    }
    // SDL_image
    // if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != 0) {
    //     SDL_Log("Failed to initialize SDL_image");
    //     return 1;
    // }

    // 创建窗口
    SDL_Window* window = SDL_CreateWindow("Chrome Dino", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    if (!window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return 1;
    }

    // 创建渲染器
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return 1;
    }
    // getchar();
    // 加载资源
    if (!Game_Load(renderer)) {
        SDL_Log("Failed to load game");
        return 1;
    }

    // 运行游戏循环
    bool quit = false;
    while (!quit) {
        Game_Init();
        Game_loop(renderer);
        quit = Game_Over(renderer);
        if (quit) {
            break;
        } else {
            Game_Reset();
        }
    }

    // 清理资源
    Game_Destroy();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
