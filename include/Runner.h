#if !defined(__RUNNER_H__)
#define __RUNNER_H__

#include <Data.h>
#include <SDL_events.h>
#include <SDL_render.h>
#include <SDL_timer.h>
#include <lTexture.h>
#include <stdbool.h>
#include <stdlib.h>

/**
 * 游戏主体类，控制游戏的整体逻辑
 */
typedef int Horizon;
typedef int HorizonLine;
typedef int Cloud;
typedef int TRex;
typedef int DistanceMeter;

typedef struct runner {

    LTexture* imageSprite; // 精灵图

    Horizon* horizon;
    TRex* tRex;
    HorizonLine* horizonLine;
    DistanceMeter* distanceMeter;

    int time; // 时钟计时器
    int currentSpeed; // 当前的速度

    bool activated; // 游戏彩蛋是否激活
    bool playing; // 游戏是否进行中
    bool crashed; // 小恐龙是否碰到了障碍物
    bool paused; // 游戏是否暂停

    void (*init)(struct runner* this, SDL_Window* gWindow, SDL_Renderer* gRender);
    void (*loop)(struct runner* this, SDL_Window* gWindow, SDL_Renderer* renderer);
    void (*handleEvent)(struct runner* this, SDL_Event* event, bool* playing);
    void (*paintEvent)(struct runner* this, SDL_Renderer* renderer);
    void (*render)(struct runner* this, SDL_Renderer* renderer);
    void (*update)(struct runner* this, SDL_Renderer* renderer);

    void (*loadImages)(struct runner* this, SDL_Renderer* renderer);
    void (*loadAudio)(struct runner* this);
    void (*setSpeed)(struct runner* this, int opt_speed);

    void (*destructor)(struct runner* this);

} Runner;

Runner* Runner_constructor();
void Runner_init(Runner* this, SDL_Window* gWindow, SDL_Renderer* gRender);

void Runner_loadImages(Runner* this, SDL_Renderer* renderer);
void Runner_loadAudio(Runner* this);
void Runner_setSpeed(Runner* this, int opt_speed);

void Runner_destructor(Runner* this);

void Runner_update(Runner* this, SDL_Renderer* renderer);

/////////////////////////////////////////////

void Runner_loop(Runner* this, SDL_Window* gWindow, SDL_Renderer* renderer);

void Runner_handleEvent(Runner* this, SDL_Event* event, bool* playing);

void Runner_render(Runner* this, SDL_Renderer* renderer);

#endif // __RUNNER_H__