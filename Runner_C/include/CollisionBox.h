#if !defined(_COLLISIONBOX_H_)
#define _COLLISIONBOX_H_
#include <SDL3/SDL.h>
#include <SDL3/SDL_rect.h>
#include <stdbool.h>


typedef struct collisionBox {
    int x;
    int y;
    int w;
    int h;
    bool (*intersects)(struct collisionBox* this, struct collisionBox* other);
} CollisionBox;

CollisionBox CollisionBox_constructor(int x, int y, int w, int h);

bool CollisionBox_intersects(CollisionBox* this, CollisionBox* other);

#endif // _COLLISIONBOX_H_