#include "../include/dino.h"
#include "../include/data.h"

Dino dino;

void Dino_Init()
{
    dino.Alive = false;
    dino.OnJump = false;
    dino.OnCrawl = false;
    dino.speed = 0;
    dino.position.x = DINO_POSTION_X;
    dino.position.y = DINO_POSTION_Y;
    dino.position.w = DINO_POSTION_W;
    dino.position.h = DINO_POSTION_H;
    dino.collision_box.x = DINO_COLLISION_X;
    dino.collision_box.y = DINO_COLLISION_Y;
    dino.collision_box.w = DINO_COLLISION_W;
    dino.collision_box.h = DINO_COLLISION_H;
}

SDL_Rect Dino_Get_Collision();

bool Dino_Alive();
void Dino_Jump();

void Dino_Update_position();
