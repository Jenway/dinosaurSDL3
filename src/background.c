#include "../include/background.h"
#include "../include/data.h"
#include "SDL_log.h"
#include <SDL_image.h>

Background background;

bool Background_Load(SDL_Renderer* renderer)
{
    background.ground_texture = IMG_LoadTexture(renderer, LAYER_GROUND_PNG);
    background.cloud_texture = IMG_LoadTexture(renderer, LAYER_CLOUD_PNG);
    background.moon_texture = IMG_LoadTexture(renderer, LAYER_MOON_PNG);
    background.star_texture = IMG_LoadTexture(renderer, LAYER_STAR_PNG);
    if (!background.ground_texture) {
        SDL_Log("Failed to create background texture: %s", SDL_GetError());
        return false;
    }

    return true;
}

void Background_Init()
{

    background.ground_position.x = BACKGROUND_POSITION_X;
    background.ground_position.y = BACKGROUND_POSITION_Y;
    background.ground_position.w = BACKGROUND_POSITION_W;
    background.ground_position.h = BACKGROUND_POSITION_H;

    background.cloud_position.x = BACKGROUND_POSITION_X;
    background.cloud_position.y = BACKGROUND_POSITION_Y;
    background.cloud_position.w = BACKGROUND_POSITION_W;
    background.cloud_position.h = BACKGROUND_POSITION_H;

    background.moon_position.x = BACKGROUND_POSITION_X;
    background.moon_position.y = BACKGROUND_POSITION_Y;
    background.moon_position.w = BACKGROUND_POSITION_W;
    background.moon_position.h = BACKGROUND_POSITION_H;

    background.star_position.x = BACKGROUND_POSITION_X;
    background.star_position.y = BACKGROUND_POSITION_Y;
    background.star_position.w = BACKGROUND_POSITION_W;
    background.star_position.h = BACKGROUND_POSITION_H;

    background.ground_speed = -BACKGROUND_SPEED;
    background.cloud_speed = -BACKGROUND_SPEED;
    background.moon_speed = -BACKGROUND_SPEED;
    background.star_speed = -BACKGROUND_SPEED;
}

void Background_Update()
{
    // SDL_Log("position.x: %d", background.position.x);
    background.ground_position.x += background.ground_speed;
    background.cloud_position.x += background.cloud_speed;
    background.moon_position.x += background.moon_speed;
    background.star_position.x += background.star_speed;
    if (background.ground_position.x <= -1600) {
        background.ground_position.x = 0;
    }
    if (background.cloud_position.x <= -1600) {
        background.cloud_position.x = 0;
    }
    if (background.moon_position.x <= -1600) {
        background.moon_position.x = 0;
    }
    if (background.star_position.x <= -1600) {
        background.star_position.x = 0;
    }
}

void Background_Draw(SDL_Renderer* renderer)
{
    // draw the road
    SDL_RenderCopy(renderer, background.ground_texture, NULL, &background.ground_position);
    SDL_RenderCopy(renderer, background.cloud_texture, NULL, &background.cloud_position);
    SDL_RenderCopy(renderer, background.moon_texture, NULL, &background.moon_position);
    SDL_RenderCopy(renderer, background.star_texture, NULL, &background.star_position);
}

void Background_Destroy()
{
    SDL_DestroyTexture(background.ground_texture);
    SDL_DestroyTexture(background.cloud_texture);
    SDL_DestroyTexture(background.moon_texture);
    SDL_DestroyTexture(background.star_texture);
}
