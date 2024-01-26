#if !defined(__TREX_H__)
#define __TREX_H__

#include "Data.h"
#include <SDL3/SDL.h>

typedef struct tRex { // frame
    enum { STEADY,
        WAITING,
        RUNNING,
        JUMPING,
        FALLING,
        DUCKING,
        CRASHED,
        NUM_STATES
    } state;
    SDL_FRect* srcRects[NUM_STATES];
    SDL_FRect* destRect;

    int subFrame;
    int currentSpeed;
    void (*destructor)(struct tRex* this);
    void (*update)(struct tRex* this);
    SDL_FRect* (*getSrcRect)(struct tRex* this);
} TRex;

TRex* TRex_constructor();
void TRex_destructor(TRex* this);

Uint32 callback_for_tRex(Uint32 interval, void* param);
void TRex_update(TRex* this);

SDL_FRect* TRex_getSrcRect(TRex* this);

#endif // __TREX_H__