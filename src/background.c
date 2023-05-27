#include "../include/background.h"
#include "../include/SDL2/SDL_image.h"
#include "../include/SDL2/SDL_surface.h"
#include "../include/data.h"
#include <stdio.h>

Background background;

bool Background_Load(SDL_Renderer* renderer)
{
    // background.texture = IMG_LoadTexture(renderer, BACKGROUND_IMAGE_PATH);
    // the bak is a bmp file, so we use SDL_LoadBMP(BACKGROUND_IMAGE_PATH);
    background.surface = SDL_LoadBMP("assets/bak.bmp");
    if (background.surface== NULL) {
        SDL_Log(" Failed to load background surface");
        return false;
    }

    background.texture = SDL_CreateTextureFromSurface(renderer, background.surface);
    if (!background.texture) {
        SDL_Log("Failed to load background texture");
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
    background.speed = BACKGROUND_SPEED;
}

void Background_Update()
{
    Background_Update_position();
}

void Background_Update_position()
{
    background.position.x -= background.speed;
    if (background.position.x <= -background.position.w) {
        background.position.x = 0;
    }
}

void Background_Draw(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, background.texture, NULL, &background.position);

    // log
    SDL_Log("background.position.x: %d, background.position.y: %d, background.position.w: %d, background.position.h: %d", background.position.x, background.position.y, background.position.w, background.position.h);
    getchar();
}

void Background_Destroy() { SDL_DestroyTexture(background.texture); }
