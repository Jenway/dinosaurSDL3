#include "../include/background.h"
#include "../include/data.h"
#include "SDL_log.h"
#include <SDL_image.h>

Background background;

bool Background_Load(SDL_Renderer* renderer)
{
    background.texture = IMG_LoadTexture(renderer, LAYER_GROUND_PNG);
    if (!background.texture) {
        SDL_Log("Failed to create background texture: %s", SDL_GetError());
        return false;
    }
    return true;
}

void Background_Init()
{
    background.position.x = BACKGROUND_POSITION_X;
    background.position.y = BACKGROUND_POSITION_Y;
    background.position.w = BACKGROUND_POSITION_W;
    background.position.h = BACKGROUND_POSITION_H;
    background.speed = -BACKGROUND_SPEED;
}

void Background_Update()
{
    // SDL_Log("position.x: %d", background.position.x);
    background.position.x += background.speed;
    if (background.position.x <= -1600) {
        background.position.x = 0;
    }
}

void Background_Draw(SDL_Renderer* renderer)
{
    // draw the road
    SDL_RenderCopy(renderer, background.texture, NULL, &background.position);
}

void Background_Destroy() { SDL_DestroyTexture(background.texture); }
