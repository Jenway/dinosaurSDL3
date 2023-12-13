#if !defined(__HORIZON_H__)
#define __HORIZON_H__

#include <Data.h>
#include <HorizonLine.h>
#include <SDL3/SDL.h>
#include <lTexture.h>

typedef struct horizon {

    HorizonLine* horizonLine;

    void (*destructor)(struct horizon* this);
    void (*update)(struct horizon* this);

    void (*draw)(struct horizon* this, SDL_Renderer* renderer, LTexture* texture);
} Horizon;

Horizon* Horizon_constructor();
void Horizon_destructor(Horizon* this);

void Horizon_draw(Horizon* this, SDL_Renderer* gRenderer, LTexture* imageSprite);

Uint32 callback_for_horizon(Uint32 interval, void* param);
void Horizon_update(Horizon* this);

#endif // __HORIZON_H__