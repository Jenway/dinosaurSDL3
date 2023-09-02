#include "../include/game.h"
#include "../include/background.h"
#include "../include/data.h"
#include "../include/dino.h"
#include "../include/obstacle.h"
#include "SDL_keycode.h"

#include <SDL_events.h>
#include <SDL_log.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_timer.h>
#include <stdbool.h>

Game game;
Dino* dino;
Obstacle* obstacle[OBSTACLE_COUNT];

void Game_loop(SDL_Renderer* renderer)
{
    // 游戏循环标志
    bool quit = false;
    // 事件循环
    SDL_Event event;

    while (!quit) {
        int Begin_Tick = SDL_GetTicks();
        // handleEvent
        Game_handleEvent(&event, &quit);
        // 更新游戏状态
        Game_Update(&quit);
        // 绘制游戏画面
        Game_Draw(renderer);
        // 显示画面
        SDL_RenderPresent(renderer);
        int End_Tick = SDL_GetTicks(); // 获得事件结束Tick
        int Delay_time = 1000 / FPS - (End_Tick - Begin_Tick);
        if (Delay_time < 0) {
            Delay_time = 0;
        }

        SDL_Delay(Delay_time);
    }
}

bool Game_Load(SDL_Renderer* renderer)
{
    // 加载背景
    if (!Background_Load(renderer)) {
        SDL_Log("Failed to load background");
        return false;
    }

    // 加载恐龙
    // malloc memory
    dino = Dino_constructor();
    if (!Dino_Load(dino, renderer)) {
        SDL_Log("Failed to load dino");
        return false;
    }
    for (int i = 0; i < OBSTACLE_COUNT; i++) {
        obstacle[i] = Obstacle_Create(2, OBSTACLE_SPEED);
        if (!obstacle[i]) {
            SDL_Log("Failed to create obstacle");
            return false;
        }
    }
    SDL_Log("Game loaded");

    return true;
}

void Game_Init()
{
    // 初始化游戏状态
    game.GAME_isOVER = false;
    game.DOWN_pressed = false;
    game.SPACE_pressed = false;
    Background_Init();
    Dino_Init(dino);
}

void Game_Reset()
{
}

void Game_handleEvent(SDL_Event* event, bool* quit)
{
    // Game_Event(event, quit);
    if (SDL_PollEvent(event)) {
        SDL_KeyCode INPUT = event->key.keysym.sym;
        switch (event->type) {
        case SDL_QUIT:
            *quit = true;
            SDL_Log("SDL_QUIT");
            break;
        case SDL_KEYDOWN:
            Game_OnKeyDown(INPUT);
            break;
        case SDL_KEYUP:
            Game_OnKeyUp(INPUT);
            break;
        default:
            break;
        }
    }
}

void Game_OnKeyDown(SDL_KeyCode INPUT)
{

    if (!game.crashed && !game.paused) {

        if (INPUT == SDLK_SPACE || INPUT == SDLK_w || INPUT == SDLK_UP) {
            // Starting the game for the first time.
            if (!game.playing) {
                // loadSounds();
                // setPlayStatus(true);
                // update();
            }
            // Start jump.
            if (!dino->jumping && !dino->ducking) {
                // if (Runner.audioCues) {
                //     this.generatedSoundFx.cancelFootSteps();
                // } else {
                //     this.playSound(this.soundFx.BUTTON_PRESS);
                // }
                int currentSpeed = DINO_JUMP_SPEED;
                Dino_StartJump(dino,currentSpeed);
            }
            // Ducking
        } else if (
            INPUT == SDLK_DOWN || INPUT == SDLK_s) {
            if (dino->jumping) {
                // Speed drop, activated only when jump key is not pressed.
                Dino_SetSpeedDrop(dino);
            } else if (!dino->jumping && !dino->ducking) {
                // Duck.
                Dino_SetDuck(dino, true);
            }
        }
    }
}

void Game_OnKeyUp(SDL_KeyCode INPUT)
{
    const SDL_KeyCode keyCode = INPUT;
    const bool isjumpKey = INPUT == SDLK_SPACE || INPUT == SDLK_w || INPUT == SDLK_UP;

    if (game.isRunning && isjumpKey) {
        Dino_EndJump(dino);
    } else if (INPUT == SDLK_DOWN || INPUT == SDLK_s) {
        dino->speedDrop = false;
        Dino_SetDuck(dino, false);
    } else if (game.crashed) {
        // Check that enough time has elapsed before allowing jump key to restart.
        // const int deltaTime = getTimeStamp() - game.time;

        //     if (this.isCanvasInView() && (Runner.keycodes.RESTART[keyCode] || this.isLeftClickOnCanvas(e) || (deltaTime >= this.config.GAMEOVER_CLEAR_TIME && Runner.keycodes.JUMP[keyCode]))) {
        //         this.handleGameOverClicks(e);
        //     }
        // } else if (this.paused && isjumpKey) { // Reset the jump state
        //     this.tRex.reset();
        //     this.play();
    }
}

void Game_Event(SDL_Event* event, bool* quit)
{
    if (SDL_PollEvent(event)) {
        SDL_KeyCode INPUT = event->key.keysym.sym;
        switch (event->type) {
        case SDL_QUIT:
            *quit = true;
            SDL_Log("SDL_QUIT");
            break;
        case SDL_KEYDOWN:
            if (INPUT == SDLK_SPACE || INPUT == SDLK_w || INPUT == SDLK_UP) {
                if (!game.SPACE_pressed) {
                    game.SPACE_pressed = true;
                }
            } else if (INPUT == SDLK_DOWN || INPUT == SDLK_s) {
                if (!game.DOWN_pressed) {
                    game.DOWN_pressed = true;
                }
            } else if (INPUT == SDLK_ESCAPE) {
                game.GAME_isOVER = true;
                *quit = true;
            }
            break;
        case SDL_KEYUP:
            if (INPUT == SDLK_SPACE || INPUT == SDLK_w || INPUT == SDLK_UP) {
                game.SPACE_pressed = false;

            } else if (INPUT == SDLK_DOWN || INPUT == SDLK_s) {
                game.DOWN_pressed = false;
                Dino_DeCrawl(dino);
            }
            break;
        default:
            break;
        }
    }
    if (game.SPACE_pressed) {
        Dino_Jump(dino);
    }
    if (game.DOWN_pressed) {
        Dino_Crawl(dino);
    }
}

void Game_Update(bool* quit)
{
    // 更新游戏状态
    Background_Update();
    // SDL_Log("Background_Update");
    Dino_Update(dino);
    for (int i = 0; i < OBSTACLE_COUNT; i++) {
        Obstacle_Update(obstacle[i]);
    }
    for (int i = 0; i < OBSTACLE_COUNT; i++) {
        if (Obstacle_Collision(obstacle[i], dino->m_collider)) {
            dino->Alive = false;
            *quit = true;
        }
    }
}

void Game_Draw(SDL_Renderer* renderer)
{
    // 绘制游戏画面
    SDL_SetRenderDrawColor(renderer, 32, 33, 36, 255);
    SDL_RenderClear(renderer);
    Background_Draw(renderer);
    Dino_Draw(dino, renderer);
    // Obstacle_Draw(renderer);
    for (int i = 0; i < OBSTACLE_COUNT; i++) {
        Obstacle_Draw(obstacle[i], renderer);
    }
}

void Game_Destroy()
{
    // 释放游戏资源
    Background_Destroy();
    Dino_destructor(dino);
    for (int i = 0; i < OBSTACLE_COUNT; i++) {
        Obstacle_Destroy(obstacle[i]);
    }
}