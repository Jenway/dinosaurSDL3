#include "../include/CollisionBox.h"
#include <SDL_rect.h>

CollisionBox CollisionBox_constructor(int x, int y, int w, int h)
{
    return (CollisionBox) {
        .x = x,
        .y = y,
        .w = w,
        .h = h,
        .intersects = CollisionBox_intersects,
    };
}

bool CollisionBox_intersects(CollisionBox* this, CollisionBox* other)
{
    SDL_Rect a = { this->x, this->y, this->w, this->h }, b = { other->x, other->y, other->w, other->h };
    return SDL_HasRectIntersection(&a, &b);
}
