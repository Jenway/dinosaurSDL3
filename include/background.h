#if !defined(__BACKGROUND_H__)
#define __BACKGROUND_H__
#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"
#include <stdbool.h>

typedef struct background {

    SDL_Rect position;
    int speed;

    SDL_Texture* texture;
    SDL_Rect offset;
    SDL_Rect cutset, cutRect;

    Uint32 FramePerSecond;
    const Uint8* keyarr;
} Background;

bool Background_Load(SDL_Renderer* renderer);
void Background_Init();
void Background_Update();
void Background_Update_position();
void Background_Draw(SDL_Renderer* renderer);
void Background_Destroy();

void Background_Quit();
void Background_DrawRoad();
void Background_DrawCloud();
void Background_DrawPlant();
void Background_DrawDino();
#endif // __BACKGROUND_H__
