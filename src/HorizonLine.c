#include "../include/HorizonLine.h"
#include "../include/CONFIG.h"
#include "SDL_rect.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

HorizonLine* HorizonLine_constructor(LTexture* canvas)
{
    HorizonLine* this = (HorizonLine*)malloc(sizeof(HorizonLine));
    this->canvas = canvas;

    this->srcRect = (SDL_Rect) { HORIZON_SRC_X, HORIZON_SRC_Y, HORIZON_SRC_WIDTH, HORIZON_SRC_HEIGHT };
    this->destRect = (SDL_Rect) { HORIZON_DEST_X, HORIZON_DEST_Y, HORIZON_DEST_WIDTH, HORIZON_DEST_HEIGHT };

    this->xPos[0] = 0;
    this->xPos[1] = 0;
    this->yPos = 0;

    this->bumpThreshold = 0.5;

    return this;
}

void HorizonLine_init(HorizonLine* this)
{
}

void HorizonLine_draw(HorizonLine* this, SDL_Renderer* renderer)
{
    LTexture_render(this->canvas, renderer, &this->srcRect, &this->destRect);
}

void HorizonLine_update(HorizonLine* this, double deltaTime, double speed)
{
    int incre = floor(speed * ((double)FPS / 1000) * deltaTime);
    
    if (this->xPos[0] <= 0) {
        HorizonLine_updateXPos(this, 0, incre);
    } else {
        HorizonLine_updateXPos(this, 1, incre);
    }
}
void HorizonLine_updateXPos(HorizonLine* this, int pos, int incre)
{
    int line1 = pos;
    int line2 = (pos == 0) ? 1 : 0;
    // 第一段地面向左移动，第二段地面随之
    this->xPos[line1] -= incre;
    this->xPos[line2] = this->xPos[line1] + this->srcRect.w;

    // 第一段地面移出了 canvas
    if (this->xPos[line1] <= -this->srcRect.w) {
        // 将第一段地面放到 canvas 右侧
        this->xPos[line1] += this->srcRect.w * 2;
        // 此时第二段地面的 x 坐标刚好和 canvas 的 x 坐标对齐
        this->xPos[line2] = this->xPos[line1] - this->srcRect.w;
        // 给放到 canvas 后面的地面随机地形
        srand(time(NULL));
        int randomType = rand() % 3;
        this->sourceXPos[line1] = randomType + this->destRect.x;
    }
}