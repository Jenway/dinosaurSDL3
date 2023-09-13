#include "../include/runner.h"
#include "../include/lTexture.h"
#include "../include/CONFIG.h"
#include "SDL_render.h"
#include <stdint.h>
Runner* Runner_constructor()
{
    Runner* this = malloc(sizeof(Runner));
    Runner_init(this);
    return this;
}

void Runner_init(Runner* this)
{

    // this->dimensions = Runner.defaultDimensions;

    this->time = 0; // 时钟计时器
    this->currentSpeed = SPEED; // 当前的速度
    this->updatePending = false; // 是否有更新待处理
    this->activated = false; // 游戏彩蛋是否被激活
    this->playing = false; // 游戏是否进行中
    this->crashed = false; // 小恐龙是否碰到了障碍物
    this->paused = false; // 游戏是否暂停

    this->spriteDef.x = 2;
    this->spriteDef.y = 54;
}
void Runner_load(Runner* this, SDL_Renderer* renderer)
{
    Runner_loadImages(this, renderer);
    this->horizon = Horizon_constructor(this->imageSprite, this->spriteDef);
    Horizon_init(this->horizon, renderer);
}
void Runner_loadImages(Runner* this,SDL_Renderer* renderer){
    // 图片在雪碧图中的坐标
    // this->spriteDef = Runner_spriteDefinition_LDPI;
    // 获取雪碧图
    this->imageSprite = LTexture_constructor();
    LTexture_loadFromFile(this->imageSprite,renderer,SPRITE_FILE_PATH);
}



void Runner_update(Runner* this,SDL_Renderer* renderer){
    this->updatePending = false; //等待更新

    // 更新时钟
    uint32_t now = SDL_GetTicks();
    double deltaTime = (now - this->time) / 1000.0;

    this->time = now;
    SDL_RenderClear(renderer);
    Horizon_update(this->horizon, deltaTime, this->currentSpeed);
    
}