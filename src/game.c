#include "../include/game.h"
#include "../include/background.h"
#include "../include/dino.h"
#include "../include/obstacle.h"

#include <SDL_events.h>
#include <SDL_log.h>
#include <SDL_render.h>
#include <SDL_timer.h>

bool Game_Load(SDL_Renderer* renderer)
{
    // 加载背景
    if (!Background_Load(renderer)) {
        SDL_Log("Failed to load background");
        return false;
    }

    // 加载恐龙
    if (!Dino_Load(renderer)) {
        SDL_Log("Failed to load dino");
        return false;
    }

    // // 加载障碍物
    // if (!Obstacle_Load(renderer)) {
    //     SDL_Log("Failed to load obstacle");
    //     return false;
    // }

    // // 加载音效
    // if (!Sound_Load()) {
    //     SDL_Log("Failed to load sound");
    //     return false;
    // }

    // // 加载字体
    // if (!Font_Load(renderer)) {
    //     SDL_Log("Failed to load font");
    //     return false;
    // }

    // // 加载分数
    // if (!Score_Load(renderer)) {
    //     SDL_Log("Failed to load score");
    //     return false;
    // }

    // // 加载游戏结束
    // if (!Gameover_Load(renderer)) {
    //     SDL_Log("Failed to load gameover");
    //     return false;
    // }

    return true;
}

void Game_loop(SDL_Renderer* renderer)
{
    // 游戏循环标志
    bool quit = false;
    // 事件循环
    SDL_Event event;

    while (!quit) {
        // 处理事件
        Game_Event(&event, &quit);
        // 更新游戏状态
        Game_Update(&quit);
        // 绘制游戏画面
        Game_Draw(renderer);
        // 显示画面
        SDL_RenderPresent(renderer);
        // 延时
        SDL_Delay(1000 / 60);
    }
}

void Game_Init()
{
    // 初始化游戏状态
    Background_Init();
    Dino_Init();
    Obstacle_Init();
    // Score_Init();
    // Gameover_Init();
}

void Game_Reset()
{
    // 重置游戏状态
    // Background_Reset();
    // Dino_Reset();
    // Obstacle_Reset();
    // Score_Reset();
    // Gameover_Reset();
}

void Game_Event(SDL_Event* event, bool* quit)
{
    while (SDL_PollEvent(event)) {
        switch (event->type) {
        case SDL_QUIT:
            *quit = true;
            break;
        case SDL_KEYDOWN:
            switch (event->key.keysym.sym) {
            case SDLK_SPACE:
                if (!Dino_Alive()) {
                    Dino_Init();
                    Obstacle_Init();
                    // Score_Init();
                    // Gameover_Init();
                }
                Dino_Jump();
                break;
            case SDLK_DOWN:
                if (!Dino_Alive()) {
                    Dino_Init();
                    Obstacle_Init();
                    // Score_Init();
                    // Gameover_Init();
                }
                Dino_Crawl();
                break;
            }
            break;
        }
    }
}

void Game_Update(bool* quit)
{
    // 更新游戏状态
    Background_Update();
    SDL_Log("Background_Update");
    Dino_Update();
    // Obstacle_Update();
    // Score_Update();
    // Gameover_Update();

    // 检测游戏结束
    if (!Dino_Alive()) {
        *quit = true;
    }
}

void Game_Draw(SDL_Renderer* renderer)
{
    // 绘制游戏画面
    SDL_RenderClear(renderer);
    
    Background_Draw(renderer);
    Dino_Draw(renderer);
    Obstacle_Draw(renderer);
    // Score_Draw();
    // Gameover_Draw();
}

void Game_Destroy()
{
    // 释放游戏资源
    // Background_Destroy();
    // Dino_Destroy();
    // Obstacle_Destroy();
    // Sound_Destroy();
    // Font_Destroy();
    // Score_Destroy();
    // Gameover_Destroy();
}

bool Game_Over(SDL_Renderer* renderer)
{
    // 检测游戏是否结束
    return !Dino_Alive();
}