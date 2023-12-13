#include <HorizonLine.h>
#include <SDL_rect.h>
#include <stdlib.h>

HorizonLine* HorizonLine_constructor()
{
    HorizonLine* this = malloc(sizeof(HorizonLine));

    this->srcRect = &(SDL_FRect) {
        .x = 446,
        .y = 5,
        .w = 30,
        .h = 12,
    };

    this->destRect = &(SDL_FRect) {
        .x = 200,
        .y = 100,
        .w = 300,
        .h = 250,
    };

    this->horizonLineX = 0;

    this->destructor = HorizonLine_destructor;
    callback_for_horizonLine(1000 / 60, this);

    return this;
}

void HorizonLine_destructor(HorizonLine* this)
{
    free(this);
    this = NULL;
}

Uint32 callback_for_horizonLine(Uint32 interval, void* param)
{
    HorizonLine* this = (HorizonLine*)param;
    // this->horizonLineX = (this->horizonLineX - 1) % 600;
    return interval;
}