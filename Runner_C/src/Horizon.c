#include <Horizon.h>
#include <stdlib.h>

Horizon* Horizon_constructor()
{
    Horizon* this = malloc(sizeof(Horizon));

    this->horizonLine = HorizonLine_constructor();

    this->destructor = Horizon_destructor;
    this->update = Horizon_update;
    this->draw = Horizon_draw;

    return this;
}

void Horizon_draw(Horizon* this, SDL_Renderer* gRenderer, LTexture* imageSprite)
{
    // TODO DRAW HORIZONLINE
    imageSprite->render(imageSprite, gRenderer, this->horizonLine->srcRect, this->horizonLine->destRect);
    // SDL_Log("Horizon_draw\n");
    // TODO DRAW CLOUDS
}

void Horizon_destructor(Horizon* this)
{
    this->horizonLine->destructor(this->horizonLine);
    free(this);
    this = NULL;
}

void Horizon_update(Horizon* this)
{
}
