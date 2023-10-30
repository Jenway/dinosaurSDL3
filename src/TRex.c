#include <RectProvider.h>
#include <SDL_rect.h>
#include <SDL_stdinc.h>
#include <locale.h>
#include <stdlib.h>
#include <tRex.h>

TRex* TRex_constructor()
{
    TRex* this = malloc(sizeof(TRex));
    this->state = 0;
    this->subFrame = 0;
    for (int i = 0; i < 9; i++) {
        this->srcRects[i] = RectProvider_getSrcRect(TREX_RECT, i);
    }
    this->destRect = RectProvider_getDestRect(TREX_RECT);

    this->destructor = TRex_destructor;
    this->update = TRex_update;
    this->getSrcRect = TRex_getSrcRect;
    return this;
}

void TRex_destructor(TRex* this)
{
    for (int i = 0; i < 9; i++) {
        free(this->srcRects[i]);
    }
    free(this->destRect);
    free(this);
    this = NULL;
}

Uint32 callback_for_tRex(Uint32 interval, void* param)
{
    TRex* this = (TRex*)param;
    this->subFrame = (this->subFrame + 1) % 2;
    return interval;
}

void TRex_update(TRex* this)
{
}

SDL_FRect* TRex_getSrcRect(TRex* this)
{
    switch (this->state) {
    case STEADY:
        return this->srcRects[0];
    case WAITING:
        return this->srcRects[1 + this->subFrame];
    case RUNNING:
        return this->srcRects[3 + this->subFrame];
    case JUMPING:
        return this->srcRects[3];
    case CRASHED:
        return this->srcRects[5 + this->subFrame];
    case DUCKING:
        return this->srcRects[7 + this->subFrame];
    }
}
