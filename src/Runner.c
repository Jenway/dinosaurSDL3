#include "../include/Runner.h"
#include "../include/Data.h"
#include <RectProvider.h>
#include <SDL_events.h>
#include <SDL_keycode.h>
#include <SDL_log.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_video.h>
#include <stdio.h>
#include <tRex.h>

Runner* Runner_constructor(SDL_Window* gWindow, SDL_Renderer* gRenderer)
{
    Runner* this = malloc(sizeof(Runner));

    this->gWindow = gWindow;
    this->gRenderer = gRenderer;
    this->imageSprite = NULL;

    this->currentSpeed = 0;
    this->horizon = 0;
    this->distanceMeter = 0;
    this->tRex = TRex_constructor();

    this->loop = Runner_loop;
    this->paintEvent = Runner_paintEvent;
    this->loadImages = Runner_loadImages;
    this->loadAudio = Runner_loadAudio;
    this->init = Runner_init;
    this->destructor = Runner_destructor;
    return this;
}

void Runner_loadImages(Runner* this)
{
    this->imageSprite = LTexture_constructor();
    LTexture_loadFromFile(this->imageSprite, this->gRenderer, SPRITE_PATH);
}

void Runner_loadAudio(Runner* this)
{
}

void Runner_setSpeed(Runner* this, int opt_speed)
{
    this->currentSpeed = opt_speed;
}

void Runner_init(Runner* this)
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

    // 8. 将画布添加到容器中
    // 9. 开始监听各种事件,如键盘输入、resize等
    // 10. 设置一个update循环来更新游戏状态和渲染
    // this->loop(this, gWindow, gRender);
    // 11. 检查深色模式,进行对应的样式调整
    // 12. 初始化高分
    // 13. 播报辅助功能描述语音
}

void Runner_loop(Runner* this)
{
    bool quit = false;
    SDL_Event e;
    while (!quit) {
        this->paintEvent(this);
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            // User requests quit
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
            if (e.type == SDL_EVENT_WINDOW_RESIZED) {
                SDL_Log("Window %d resized to %dx%d\n", e.window.windowID, e.window.data1, e.window.data2);
                // this->paintEvent(this);
            }
            if (e.type == SDL_EVENT_KEY_DOWN) {
                const SDL_Keycode keyName = e.key.keysym.sym;
                if (keyName == SDLK_ESCAPE || keyName == SDLK_q) {
                    quit = true;
                }
                if (e.key.keysym.sym == SDLK_SPACE) {
                    SDL_Log("Window %d resized to %dx%d\n", e.window.windowID, e.window.data1, e.window.data2);
                }
            }
        }
    }
}

void Runner_paintEvent(Runner* this)
{
    // Clear screen
    SDL_RenderClear(this->gRenderer);
    // draw bak color f7f7f7
    SDL_SetRenderDrawColor(this->gRenderer, 0xf7, 0xf7, 0xf7, 0x7f);
    SDL_RenderFillRect(this->gRenderer, NULL);
    SDL_RendererFlip flip = SDL_FLIP_NONE;

    this->imageSprite->render(this->imageSprite, this->gRenderer, this->tRex->srcRect, this->tRex->destRect);

    // this->imageSprite->render(this->imageSprite, this->gRenderer, NULL, dest_rect);

    // Update screen
    SDL_RenderPresent(this->gRenderer);
}

void Runner_destructor(Runner* this)
{
    LTexture_destructor(this->imageSprite);
    TRex_destructor(this->tRex);

    free(this);
}