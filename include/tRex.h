#if !defined(__TREX_H__)
#define __TREX_H__

#include <Data.h>
#include <SDL3/SDL.h>

typedef struct tRex {
    SDL_FRect* srcRect;
    SDL_FRect* destRect;
    void (*destructor)(struct tRex* this);
} TRex;

TRex* TRex_constructor();
void TRex_destructor(TRex* this);

#endif // __TREX_H__