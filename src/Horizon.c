#include "../include/Horizon.h"
#include "../include/CONFIG.h"
#include "SDL_rect.h"
#include <stdlib.h>

Horizon* Horizon_constructor(LTexture* sprite, SDL_Rect spritePos)
{
    Horizon* this = (Horizon*)malloc(sizeof(Horizon));
    this->spritePos = spritePos;
    this->canvas = sprite;
    SDL_Rect sprite_rect;
    sprite_rect.x = HORIZON_SRC_X;
    sprite_rect.y = HORIZON_SRC_Y;
    return this;
}

void Horizon_init(Horizon* this, SDL_Renderer* renderer)
{
    this->horizonLine = HorizonLine_constructor(this->canvas);
}

void Horizon_render(Horizon* this,SDL_Renderer* renderer){
    HorizonLine_draw(this->horizonLine, renderer);
}


void Horizon_update(Horizon* this, double deltaTime, double speed)
{
    HorizonLine_update(this->horizonLine, deltaTime, speed);
}