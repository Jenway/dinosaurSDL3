#include <RectProvider.h>
#include <SDL_rect.h>
#include <SDL_stdinc.h>
#include <locale.h>
#include <stdlib.h>
#include <tRex.h>

TRex* TRex_constructor()
{
    TRex* this = malloc(sizeof(TRex));
    this->srcRect = RectProvider_getSrcRect(TREX_RECT);
    this->destRect = RectProvider_getDestRect(TREX_RECT);
    this->destructor = TRex_destructor;
    return this;
}

void TRex_destructor(TRex* this)
{
    free(this->srcRect);
    free(this->destRect);
    free(this);
    this = NULL;
}