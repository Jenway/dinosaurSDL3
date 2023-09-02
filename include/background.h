#include "SDL_render.h"
#if !defined(__BACKGROUND_H__)
#define __BACKGROUND_H__
#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"
#include <stdbool.h>

typedef struct background {

    SDL_Rect ground_position;
    SDL_Rect cloud_position;
    SDL_Rect moon_position;
    SDL_Rect star_position;

    int ground_speed;
    int cloud_speed;
    int moon_speed;
    int star_speed;

    SDL_Texture* ground_texture;
    SDL_Texture* cloud_texture;
    SDL_Texture* moon_texture;
    SDL_Texture* star_texture;

} Background;

Background* Background_Create(int index, int Speed);
bool Background_Load(SDL_Renderer* renderer);
void Background_Init();
void Background_Update();
void Background_Draw(SDL_Renderer* renderer);
void Background_Destroy();

#endif // __BACKGROUND_H__
