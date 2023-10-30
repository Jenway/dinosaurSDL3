#include "../include/RectProvider.h"
#include <Data.h>

RectProvider RectProvider_constructor()
{
    return (RectProvider) {
        .getSrc = RectProvider_getSrcRect,
        .getDest = RectProvider_getDestRect,
    };
}

SDL_FRect* RectProvider_getSrcRect(int choice)
{
    SDL_FRect* src_rect = malloc(sizeof(SDL_FRect));
    switch (choice) {
    case TREX_RECT:
        *src_rect = (SDL_FRect)SPRITE_TREX_RUNNING_1;
        break;
    case CACTUS_RECT:
        src_rect->x = 446;
        src_rect->y = 2;
        src_rect->w = 17;
        src_rect->h = 35;
        break;
    case BIRD_RECT:
        src_rect->x = 260;
        src_rect->y = 0;
        src_rect->w = 46;
        src_rect->h = 40;
        break;
    case GROUND_RECT:
        src_rect->x = 2;
        src_rect->y = 54;
        src_rect->w = 2400;
        src_rect->h = 26;
        break;
    case CLOUD_RECT:
        src_rect->x = 86;
        src_rect->y = 2;
        src_rect->w = 46;
        src_rect->h = 14;
        break;
    // case BUTTON_RECT:
    //     src_rect->x = 2;
    //     src_rect->y = 2;
    //     src_rect->w = 72;
    //     src_rect->h = 64;
    default:
        src_rect->x = 0;
        src_rect->y = 0;
        src_rect->w = 0;
        src_rect->h = 0;
        break;
    }
    return src_rect;
}
SDL_FRect RectProvider_MultiplyRect(SDL_FRect* rect, float factor)
{
    return (SDL_FRect) {
        .x = rect->x * factor,
        .y = rect->y * factor,
        .w = rect->w * factor,
        .h = rect->h * factor,
    };
}

SDL_FRect* RectProvider_getDestRect(int choice)
{
    SDL_FRect* dest_rect = malloc(sizeof(SDL_FRect));
    switch (choice) {
    case TREX_RECT:
        dest_rect->x = 88 * 2;
        dest_rect->y = 6 * 2;
        dest_rect->w = 88 * 2;
        dest_rect->h = 90 * 2;
        break;
    case CACTUS_RECT:
        dest_rect->x = 446;
        dest_rect->y = 2;
        dest_rect->w = 17;
        dest_rect->h = 35;
        break;
    case BIRD_RECT:
        dest_rect->x = 260;
        dest_rect->y = 0;
        dest_rect->w = 46;
        dest_rect->h = 40;
        break;
    case GROUND_RECT:
        dest_rect->x = 2;
        dest_rect->y = 54;
        dest_rect->w = 2400;
        dest_rect->h = 26;
        break;
    case CLOUD_RECT:
        dest_rect->x = 86;
        dest_rect->y = 2;
        dest_rect->w = 46;
        dest_rect->h = 14;
        break;
    default:
        dest_rect->x = 0;
        dest_rect->y = 0;
        dest_rect->w = 0;
        dest_rect->h = 0;
        break;
    }
    return dest_rect;
}
