#include "../include/dino.h"
#include "../include/data.h"
#include <SDL_image.h>
#include <SDL_log.h>

Dino dino;

bool Dino_Load(SDL_Renderer* renderer)
{
    dino.texture[0] = IMG_LoadTexture(renderer, DINO_IMAGE_PATH);
    dino.texture[1] = IMG_LoadTexture(renderer, DINO_IMAGE_PATH2);
    dino.texture[2] = IMG_LoadTexture(renderer, DINO_IMAGE_PATH3);
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
    dino.speed = 0;
    dino.OnJump = false;
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
    SDL_RenderCopy(renderer, dino.texture[dino.frame], NULL, &dino.position);
}

void Dino_Update_position()
{
    if (dino.OnJump) {
        dino.position.y -= dino.speed;
        dino.speed -= DINO_GRAVITY;
        if (dino.position.y >= DINO_POSTION_Y) {
            dino.position.y = DINO_POSTION_Y;
            dino.OnJump = false;
        }
    }
}

void Dino_Update_Frame()
{
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

void Dino_Jump()
{
    if (!dino.OnJump) {
        dino.OnJump = true;
        dino.speed = DINO_JUMP_SPEED;
    }
}

void Dino_Crawl()
{
    dino.position.h = DINO_POSTION_H / 2;
    dino.collision_box.h = DINO_POSTION_H / 2 - 20;
}
