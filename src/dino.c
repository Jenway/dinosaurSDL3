#include "../include/dino.h"
#include "../include/data.h"
#include "SDL_render.h"
#include <SDL_image.h>
#include <SDL_log.h>
#include <stdbool.h>
#include <stdio.h>

Dino dino;

bool Dino_Load(SDL_Renderer* renderer)
{
    // load the spirite sheet as a texture
    // cut the dino image from the sprite sheet
    // load the dino image as a texture

    SDL_Texture* sprite_sheet = IMG_LoadTexture(renderer, SPRITE_SHEET_PATH);
    if (!sprite_sheet) {
        SDL_Log("Failed to load sprite sheet");
        return false;
    }

    SDL_Rect dino_rect_1 = { SPRITE_DINO_RUN_1_X, SPRITE_DINO_RUN_1_Y,
        SPRITE_DINO_RUN_1_W, SPRITE_DINO_RUN_1_H };

    SDL_Rect dino_rect_2 = { SPRITE_DINO_RUN_2_X, SPRITE_DINO_RUN_2_Y,
        SPRITE_DINO_RUN_2_W, SPRITE_DINO_RUN_2_H };

    // cut the dino image from the sprite sheet

    dino.texture[0] = IMG_LoadTexture(renderer, DINO_IMAGE_PATH1);
    dino.texture[1] = IMG_LoadTexture(renderer, DINO_IMAGE_PATH2);
    dino.texture[2] = IMG_LoadTexture(renderer, DINO_IMAGE_PATH3);
    dino.texture[3] = IMG_LoadTexture(renderer, DINO_IMAGE_PATH4);

    if (!dino.texture[0]) {
        SDL_Log("Failed to load dino texture");
        return false;
    }
    return true;
}

void Dino_Init()
{
    dino.position.x = DINO_POSTION_X;
    dino.position.y = DINO_POSTION_Y;
    dino.position.w = DINO_POSTION_W;
    dino.position.h = DINO_POSTION_H;
    dino.collision_box.x = DINO_POSTION_X + 10;
    dino.collision_box.y = DINO_POSTION_Y + 10;
    dino.collision_box.w = DINO_POSTION_W - 20;
    dino.collision_box.h = DINO_POSTION_H - 20;
    dino.y_velocity = 0;
    dino.OnAir = false;
    dino.Alive = true;
    dino.frame_timer = DINO_FRAME_CHANGE_TIME;
}

void Dino_Update()
{
    Dino_Update_position();
    Dino_Update_Frame();
}

void Dino_Draw(SDL_Renderer* renderer)
{
    if (!dino.OnCrawl) {
        SDL_RenderCopy(renderer, dino.texture[dino.frame], NULL, &dino.position);
    } else {
        SDL_RenderCopy(renderer, dino.texture[dino.frame + 2], NULL, &dino.position);
    }
}
void Dino_Jump()
{

    if (!dino.OnAir && dino.y_velocity != 0) {
        dino.y_velocity = DINO_JUMP_SPEED;
        dino.OnAir = true;
    }
}

void Dino_Update_position()
{
    // if not jumping or falling, do nothing
    if (!dino.OnAir && dino.y_velocity != 0) {
        return;
    }
    // if jumping, update y position
    bool condition = dino.position.y + (dino.y_velocity / FPS) > DINO_POSTION_Y;
    if (condition) {
        dino.position.y = DINO_POSTION_Y;
        dino.y_velocity = 0;
        dino.OnAir = false;
        return;
    }

    dino.y_velocity += DINO_GRAVITY;
    dino.position.y += dino.y_velocity / FPS;
}

void Dino_Update_Frame()
{
    if (dino.OnAir == true) {
        dino.frame = 0;
        return;
    }
    // 更新计时器
    dino.frame_timer -= DINO_FRAME_DELTA_TIME;

    if (dino.frame_timer <= 0) {
        // 切换恐龙的动画帧

        dino.frame = (dino.frame + 1) % DINO_FRAME_COUNT;

        dino.frame_timer = 200;
    }
}

SDL_Rect Dino_Get_Collision() { return dino.collision_box; }

bool Dino_Alive() { return dino.Alive; }

void Dino_Crawl()
{
    // change collicsxbox
    if (!dino.OnAir) {
        dino.OnCrawl = true;
    }
    if (dino.OnAir) {
        dino.y_velocity = DINO_FALL_SPEED;
    }
}

void Dino_DeCrawl()
{
    // change collicsxbox
    dino.OnCrawl = false;
}