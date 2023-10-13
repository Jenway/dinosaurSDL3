#include "../include/RectProvider.h"

RectProvider RectProvider_constructor()
{
    return (RectProvider) {
        .getSrc = RectProvider_getSrcRect,
        .getDest = RectProvider_getDestRect,
    };
}

SDL_Rect* RectProvider_getSrcRect(int choice)
{
    SDL_Rect* src_rect = malloc(sizeof(SDL_Rect));
    switch (choice) {
    case TREX_RECT:
        src_rect->x = 0;
        src_rect->y = 0;
        src_rect->w = 44;
        src_rect->h = 47;
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
    default:
        src_rect->x = 0;
        src_rect->y = 0;
        src_rect->w = 0;
        src_rect->h = 0;
        break;
    }
    return src_rect;
}

SDL_Rect* RectProvider_getDestRect(int choice)
{
    SDL_Rect* dest_rect = malloc(sizeof(SDL_Rect));
    switch (choice) {
    case TREX_RECT:
        dest_rect->x = 0;
        dest_rect->y = 0;
        dest_rect->w = 44;
        dest_rect->h = 47;
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
