#include <SDL3/SDL.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include <SDL3_image/SDL_image.h>
#include <SDL_oldnames.h>
#include <SDL_stdinc.h>
#include <stdbool.h>
#include <stdio.h>

#include "../include/runner.h"

// Screen dimension constants
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 150;

#define SCREEN_TITLE "Chrome Dino"

// Starts up SDL and creates window
bool init();
void close();

// The window we'll be rendering to
SDL_Window* gWindow = NULL;
// The window renderer
SDL_Renderer* gRenderer = NULL;

int main(int argc, char* argv[])
{
    if (!init()) {
        SDL_Log("Failed to initialize!\n");
    } else {
        Runner* this = Runner_constructor();
        // load resources
        if (!Runner_Load(this, gRenderer)) {
            SDL_Log("Failed to load game");
            return 1;
        }
        // 运行游戏循环
        bool quit = false;
        while (!quit) {
            Game_Init(this);
            SDL_Log("Game Initialized");
            Runner_loop(this, gRenderer);
            SDL_Log("Game Over");
            quit = true;
            if (quit) {
                break;
            } else {
                Game_Reset();
                SDL_Log("Game Reset");
            }
        }
    }
    // Free resources and close SDL
    close();
    return 0;
}

bool init()
{
    // Initialization flag
    bool success = true;

    int SDL_Init_Flag = SDL_INIT_VIDEO | SDL_INIT_AUDIO;
    // Initialize SDL
    if (SDL_Init(SDL_Init_Flag) != 0) {
        SDL_Log("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    } else {
        // Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
            SDL_Log("Warning: Linear texture filtering not enabled!");
        }

        // Create window
        gWindow = SDL_CreateWindow(SCREEN_TITLE, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_EVENT_WINDOW_RESIZED);
        if (gWindow == NULL) {
            SDL_Log("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        } else {
            Uint32 SDL_CreateRenderer_Flag = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
            // Create renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, SCREEN_TITLE, SDL_CreateRenderer_Flag);
            if (gRenderer == NULL) {
                SDL_Log("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            } else {
                // Initialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                // Initialize PNG && JPG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    SDL_Log("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }
            }
        }
    }
    return success;
}

void close()
{
    // Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    // Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}
