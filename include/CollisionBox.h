#include <stdbool.h>
#if !defined(_COLLISIONBOX_H_)
#define _COLLISIONBOX_H_
#include <SDL3/SDL_rect.h>

typedef struct collisionBox {
    SDL_Rect box;
    int x;
    int y;
    int w;
    int h;
    bool (*checkCollision)(struct collisionBox* this, struct collisionBox* other);
} CollisionBox;

CollisionBox* CollisionBox_constructor();
void CollisionBox_destructor(CollisionBox* this);
bool CollisionBox_checkCollision(CollisionBox* this, CollisionBox* other);

#endif // _COLLISIONBOX_H_