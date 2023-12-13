#include <SDL_stdinc.h>
#include <stdint.h>
#if !defined(__HORIZONLINE_H__)
#define __HORIZONLINE_H__

#include <Data.h>
#include <SDL3/SDL.h>

typedef struct horizonLine {
    SDL_FRect* srcRect;
    SDL_FRect* destRect;

    Uint32 horizonLineX;

    void (*destructor)(struct horizonLine* this);
    void (*update)(struct horizonLine* this);

} HorizonLine;

HorizonLine* HorizonLine_constructor();
void HorizonLine_destructor(HorizonLine* this);

Uint32 callback_for_horizonLine(Uint32 interval, void* param);

#endif // __HORIZONLINE_H__