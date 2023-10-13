#include "../include/Runner.h"
#include "../include/Data.h"
#include <SDL_log.h>
#include <SDL_rect.h>
#include <SDL_video.h>
#include <stdio.h>

Runner* Runner_constructor()
{
    Runner* this = malloc(sizeof(Runner));
    this->imageSprite = NULL;
    this->currentSpeed = 0;
    this->horizon = 0;
    this->distanceMeter = 0;
    this->tRex = 0;
    this->loop = Runner_loop;
    this->loadImages = Runner_loadImages;
    this->loadAudio = Runner_loadAudio;
    this->init = Runner_init;
    this->destructor = Runner_destructor;
    return this;
}

void Runner_loadImages(Runner* this, SDL_Renderer* renderer)
{
    this->imageSprite = LTexture_constructor();
    LTexture_loadFromFile(this->imageSprite, renderer, SPRITE_PATH);
}

void Runner_loadAudio(Runner* this)
{
}

void Runner_setSpeed(Runner* this, int opt_speed)
{
    this->currentSpeed = opt_speed;
}

void Runner_init(Runner* this, SDL_Window* gWindow, SDL_Renderer* gRender)
{
    //     1. 隐藏静态图标
    // 2. 计算并调整游戏画布尺寸
    // 3. 设置T-Rex速度
    Runner_setSpeed(this, 6);
    // 4. 创建游戏容器、画布、live region等DOM元素
    // 5. 生成音频Context和声音效果对象
    // 6. 获得画布context,填充画布的背景
    // 7. 创建游戏场景中的各个对象,如地平线、距离表、恐龙等
    this->horizon = 0;
    this->distanceMeter = 0;
    this->tRex = 0;

    // 8. 将画布添加到容器中
    // 9. 开始监听各种事件,如键盘输入、resize等
    // 10. 设置一个update循环来更新游戏状态和渲染
    // this->loop(this, gWindow, gRender);
    // 11. 检查深色模式,进行对应的样式调整
    // 12. 初始化高分
    // 13. 播报辅助功能描述语音
}

void Runner_loop(Runner* this, SDL_Window* gWindow, SDL_Renderer* gRenderer)
{
    SDL_FRect* dest_rect = &((SDL_FRect) { 0, 0, 3200, 600 });
    // Clear screen
    SDL_RenderClear(gRenderer);

    this->imageSprite->render(this->imageSprite, gRenderer, NULL, dest_rect);

    // Update screen
    SDL_RenderPresent(gRenderer);
    getchar();
}

void Runner_destructor(Runner* this)
{
    LTexture_destructor(this->imageSprite);
    free(this);
}