#if !defined(_RECTPROVIDER_H_)
#define _RECTPROVIDER_H_
#include <SDL3/SDL.h>
#include <SDL3/SDL_rect.h>
#include <stdlib.h>

/*
 *  This struct is used to provide the SDL_Rect for the game objects
 *
 *  1 - tRex 2 - cactus 3 - bird 4 - ground 5 - cloud
 */
typedef struct rectProvider {
    SDL_FRect* (*getSrc)(int choice);
    SDL_FRect* (*getDest)(int choice);
} RectProvider;

enum {
    TREX_RECT = 1,
    CACTUS_RECT,
    BIRD_RECT,
    GROUND_RECT,
    CLOUD_RECT
};

RectProvider RectProvider_constructor();

SDL_FRect* RectProvider_getSrcRect(int choice);

SDL_FRect RectProvider_MultiplyRect(SDL_FRect* rect, float factor);

SDL_FRect* RectProvider_getDestRect(int choice);

#endif // _RECTPROVIDER_H_