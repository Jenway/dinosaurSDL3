#include "../include/RectProvider.h"
#include <Data.h>

RectProvider RectProvider_constructor()
{
    return (RectProvider) {
        .getSrc = RectProvider_getSrcRect,
        .getDest = RectProvider_getDestRect,
    };
}

SDL_FRect* RectProvider_getSrcRect(int choice, int state)
{
    SDL_FRect* src_rect = malloc(sizeof(SDL_FRect));
    switch (choice) {
    case TREX_RECT:
        switch (state) {
        case 0:
            *src_rect = (SDL_FRect)SPRITE_TREX_WATING_STEAY;
            break;
        case 1:
            *src_rect = (SDL_FRect)SPRITE_TREX_WATING_1;
            break;
        case 2:
            *src_rect = (SDL_FRect)SPRITE_TREX_WATING_2;
            break;
        case 3:
            *src_rect = (SDL_FRect)SPRITE_TREX_RUNNING_1;
            break;
        case 4:
            *src_rect = (SDL_FRect)SPRITE_TREX_RUNNING_2;
            break;
        case 5:
            *src_rect = (SDL_FRect)SPRITE_TREX_CRASHED_1;
            break;
        case 6:
            *src_rect = (SDL_FRect)SPRITE_TREX_CRASHED_2;
            break;
        case 7:
            *src_rect = (SDL_FRect)SPRITE_TREX_DUCKING_1;
            break;
        case 8:
            *src_rect = (SDL_FRect)SPRITE_TREX_DUCKING_2;
            break;
        default:
            break;
        }

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
        *dest_rect = (SDL_FRect) {
            .x = 50 * 2,
            .y = 50 * 2,
            .w = 88 * 2,
            .h = 94 * 2,
        };
        break;
    default:
        break;
    }
    return dest_rect;
}
