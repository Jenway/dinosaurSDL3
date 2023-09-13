#if !defined(__HORIZONLINE_H__)
#define __HORIZONLINE_H__
#include "lTexture.h"
#include <SDL_rect.h>

typedef struct horizonLine {
    LTexture* canvas;
    
    SDL_Rect srcRect; //SOURCE
    SDL_Rect destRect; //DESTINATION
    // void* sourceXPos;
    int xPos[2];
    int sourceXPos[2];
    int yPos;

    double bumpThreshold;

} HorizonLine;

HorizonLine* HorizonLine_constructor(LTexture* canvas);
void HorizonLine_init(HorizonLine* this);
void HorizonLine_draw(HorizonLine* this, SDL_Renderer* renderer);
void HorizonLine_update(HorizonLine* this, double deltaTime, double speed);

void HorizonLine_updateXPos(HorizonLine* this, int pos, int incre);
void HorizonLine_destructor(HorizonLine* this);

#endif // __HORIZONLINE_H__