#if !defined(__GAME_H__)
#define __GAME_H__
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_surface.h"
#include "SDL2/SDL_video.h"
#include <stdbool.h>

typedef struct game {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Surface* dino;
    SDL_Surface* road;
    SDL_Surface* cloud;
    SDL_Surface* plant;
    // keyboard state
    bool SPACE_pressed;
    bool DOWN_pressed;
    bool GAME_isOVER;
    Uint32 FramePerSecond;
    const Uint8* keyarr;
} Game;

bool Game_Load(SDL_Renderer* renderer);
void Game_loop(SDL_Renderer* renderer);
void Game_Init();
void Game_Reset();
void Game_Event(SDL_Event* event, bool* quit);
void Game_Update(bool* quit);
void Game_Draw(SDL_Renderer* renderer);
void Game_Destroy();
bool Game_Over(SDL_Renderer* renderer);

#endif // __GAME_H__
