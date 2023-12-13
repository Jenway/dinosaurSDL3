#if !defined(__RUNNER_H__)
#define __RUNNER_H__

#include "tRex.h"
#include <Data.h>
#include <Horizon.h>
#include <SDL_events.h>
#include <SDL_render.h>
#include <SDL_timer.h>
#include <lTexture.h>
#include <stdbool.h>
#include <stdlib.h>


/**
 * 游戏主体类，控制游戏的整体逻辑
 */

typedef int DistanceMeter;

typedef struct runner {

    SDL_Window* gWindow;
    SDL_Renderer* gRenderer;
    LTexture* imageSprite; // 精灵图

    Horizon* horizon;
    TRex* tRex;
    DistanceMeter* distanceMeter;

    int time; // 时钟计时器
    int currentSpeed; // 当前的速度

    bool activated; // 游戏彩蛋是否激活
    bool playing; // 游戏是否进行中
    bool crashed; // 小恐龙是否碰到了障碍物
    bool paused; // 游戏是否暂停

    void (*init)(struct runner* this);
    void (*loop)(struct runner* this);
    void (*handleEvent)(struct runner* this, SDL_Event* event, bool* playing);
    void (*paintEvent)(struct runner* this);
    void (*render)(struct runner* this);
    void (*update)(struct runner* this);

    void (*loadImages)(struct runner* this);
    void (*loadAudio)(struct runner* this);
    void (*setSpeed)(struct runner* this, int opt_speed);

    void (*destructor)(struct runner* this);

} Runner;

Runner* Runner_constructor(SDL_Window* gWindow, SDL_Renderer* gRenderer);
void Runner_init(Runner* this);

void Runner_loadImages(Runner* this);
void Runner_loadAudio(Runner* this);
void Runner_setSpeed(Runner* this, int opt_speed);

void Runner_destructor(Runner* this);

void Runner_update(Runner* this);

/////////////////////////////////////////////

void Runner_loop(Runner* this);

void Runner_handleEvent(Runner* this, SDL_Event* event, bool* playing);

void Runner_paintEvent(Runner* this);

void Runner_render(Runner* this);

#endif // __RUNNER_H__