#if !defined(__GAMEOVER_H__)
#define __GAMEOVER_H__

#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"

#include <stdbool.h>

typedef struct gameover {
    SDL_Rect position;
    SDL_Texture* texture;
    SDL_Renderer* renderer;
} Gameover;

bool Gameover_Load(SDL_Renderer* renderer);
void Gameover_Update();
void Gameover_Draw(SDL_Renderer* renderer);
void Gameover_Destroy();
bool Gameover_Restart();

#endif // __GAMEOVER_H__