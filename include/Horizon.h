#include "SDL_render.h"
#if !defined(__HORIZON_H__)
#define __HORIZON_H__
#include <SDL_rect.h>

#include "HorizonLine.h"
#include "lTexture.h"



typedef struct horizon {
    LTexture* canvas;
    void* ctx;
    SDL_Rect spritePos;
    HorizonLine* horizonLine;

} Horizon;

Horizon* Horizon_constructor(LTexture* sprite, SDL_Rect spritePos);
void Horizon_init(Horizon* this,SDL_Renderer* renderer);
void Horizon_render(Horizon* this,SDL_Renderer* renderer);

void Horizon_update(Horizon* this, double deltaTime, double speed);


void Horizon_destructor(Horizon* this);

#endif // __HORIZON_H__