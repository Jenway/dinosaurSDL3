#include "../include/dino.h"
#include "../include/data.h"
#include "../include/game.h"
#include "../include/lTexture.h"
#include "SDL_render.h"
#include <SDL_image.h>
#include <SDL_log.h>
#include <stdbool.h>
#include <stdio.h>

extern Game* game;
Dino* Dino_constructor()
{
    Dino* dino = malloc(sizeof(Dino));
    return dino;
}

bool Dino_Load(Dino* dino, SDL_Renderer* renderer)
{

    SDL_Texture* sprite_sheet = IMG_LoadTexture(renderer, SPRITE_SHEET_PATH);
    if (!sprite_sheet) {
        SDL_Log("Failed to load sprite sheet");
        return false;
    }

    dino->texture[0] = IMG_LoadTexture(renderer, DINO_IMAGE_PATH1);
    dino->texture[1] = IMG_LoadTexture(renderer, DINO_IMAGE_PATH2);
    dino->texture[2] = IMG_LoadTexture(renderer, DINO_IMAGE_PATH3);
    dino->texture[3] = IMG_LoadTexture(renderer, DINO_IMAGE_PATH4);
    dino->texture[4] = IMG_LoadTexture(renderer, DINO_IMAGE_PATH5);

    if (!dino->texture[0]) {
        SDL_Log("Failed to load dino texture");
        return false;
    }
    return true;
}

void Dino_Init(Dino* dino)
{
    dino->position.x = DINO_POSTION_X;
    dino->position.y = DINO_POSTION_Y;
    dino->position.w = DINO_POSTION_W;
    dino->position.h = DINO_POSTION_H;
    dino->m_collider.x = DINO_POSTION_X + 10;
    dino->m_collider.y = DINO_POSTION_Y + 10;
    dino->m_collider.w = DINO_POSTION_W - 20;
    dino->m_collider.h = DINO_POSTION_H - 20;
    dino->y_velocity = 0;
    dino->OnAir = false;
    dino->Alive = true;
    dino->frame_timer = DINO_FRAME_CHANGE_TIME;
}

bool Dino_Collision(Dino* dino, SDL_Rect* rect)
{
    SDL_Rect dino_rect = Dino_Get_Collision(dino);
    return SDL_HasIntersection(rect, &dino_rect);
}

void Dino_handleEvent(Dino* dino, SDL_Event* event, bool* quit)
{
}

void Dino_Update(Dino* dino)
{
    Dino_Update_position(dino);
    Dino_Update_Frame(dino);
}

void Dino_Draw(Dino* dino, SDL_Renderer* renderer)
{
    if (dino->OnAir) {
        SDL_RenderCopy(renderer, dino->texture[4], NULL, &dino->position);
        return;
    }
    if (!dino->OnCrawl) {
        SDL_RenderCopy(renderer, dino->texture[dino->frame], NULL, &dino->position);
    } else {
        SDL_RenderCopy(renderer, dino->texture[dino->frame + 2], NULL, &dino->position);
    }
}
void Dino_Jump(Dino* dino)
{

    if (!dino->OnAir && dino->y_velocity != 0) {
        dino->y_velocity = DINO_JUMP_SPEED;
        dino->OnAir = true;
    }
}

void Dino_StartJump(Dino* this, int speed)
{

    if (!this->OnAir) {
        // this.update(0, Trex.status.JUMPING);
        // Tweak the jump velocity based on the speed.
        // this.jumpVelocity = this.config.INITIAL_JUMP_VELOCITY - (speed / 10);
        this->y_velocity = DINO_JUMP_SPEED;
        this->jumping = true;
        this->reachedMinHeight = false;
        this->speedDrop = false;
    }
}
void Dino_EndJump(Dino* this)
{
    if (this->reachedMinHeight && this->y_velocity < DINO_FALL_SPEED) {
        this->y_velocity = DINO_FALL_SPEED;
    }
}
void Dino_UpdateJump(Dino* this, int deltaTime)
{
    // const msPerFrame = Trex.animFrames[this.status].msPerFrame;
    // const framesElapsed = deltaTime / msPerFrame;

    // // Speed drop makes Trex fall faster.
    // if (this.speedDrop) {
    //     this.yPos += Math.round(this.jumpVelocity * this.config.SPEED_DROP_COEFFICIENT * framesElapsed);
    // } else if (this.config.INVERT_JUMP) {
    //     this.yPos -= Math.round(this.jumpVelocity * framesElapsed);
    // } else {
    //     this.yPos += Math.round(this.jumpVelocity * framesElapsed);
    // }

    // this.jumpVelocity += this.config.GRAVITY * framesElapsed;

    // // Minimum height has been reached.
    // if (this.config.INVERT_JUMP && (this.yPos > this.minJumpHeight) || !this.config.INVERT_JUMP && (this.yPos < this.minJumpHeight) || this.speedDrop) {
    //     this.reachedMinHeight = true;
    // }

    // // Reached max height.
    // if (this.config.INVERT_JUMP && (this.yPos > -this.config.MAX_JUMP_HEIGHT) || !this.config.INVERT_JUMP && (this.yPos < this.config.MAX_JUMP_HEIGHT) || this.speedDrop) {
    //     this.endJump();
    // }

    // // Back down at ground level. Jump completed.
    // if ((this.config.INVERT_JUMP && this.yPos) < this.groundYPos || (!this.config.INVERT_JUMP && this.yPos) > this.groundYPos) {
    //     this.reset();
    //     this.jumpCount++;

    //     if (Runner.audioCues) {
    //         Runner.generatedSoundFx.loopFootSteps();
    //     }
    // }
}

/**
 * Set the speed drop. Immediately cancels the current jump.
 */
void Dino_SetSpeedDrop(Dino* this) { }

// Dino Duck

void Dino_SetDuck(Dino* this, bool isDucking) { }

void Dino_Update_position(Dino* dino)
{
    // if not jumping or falling, do nothing
    if (!dino->OnAir && dino->y_velocity != 0) {
        return;
    }
    // if jumping, update y position
    bool condition = dino->position.y + (dino->y_velocity / FPS) > DINO_POSTION_Y;
    if (condition) {
        dino->position.y = DINO_POSTION_Y;
        dino->y_velocity = 0;
        dino->OnAir = false;
        return;
    }

    dino->y_velocity += DINO_GRAVITY;
    dino->position.y += dino->y_velocity / FPS;
}

void Dino_Update_Frame(Dino* dino)
{
    if (dino->OnAir == true) {
        dino->frame = 0;
        return;
    }
    // 更新计时器
    dino->frame_timer -= DINO_FRAME_DELTA_TIME;

    if (dino->frame_timer <= 0) {
        // 切换恐龙的动画帧

        dino->frame = (dino->frame + 1) % DINO_FRAME_COUNT;

        dino->frame_timer = 200;
    }
}

SDL_Rect Dino_Get_Collision(Dino* dino) { return dino->m_collider; }

bool Dino_Alive(Dino* dino) { return dino->Alive; }

void Dino_Crawl(Dino* dino)
{
    // change collicsxbox
    if (!dino->OnAir) {
        dino->OnCrawl = true;
    }
    if (dino->OnAir) {
        dino->y_velocity = DINO_FALL_SPEED;
    }
}

void Dino_DeCrawl(Dino* dino)
{
    // change collicsxbox
    dino->OnCrawl = false;
}

void Dino_destructor(Dino* dino)
{
    Dino_Destroy(dino);
    free(dino);
}
void Dino_Destroy(Dino* dino)
{
    SDL_DestroyTexture(dino->texture[0]);
    SDL_DestroyTexture(dino->texture[1]);
    SDL_DestroyTexture(dino->texture[2]);
    SDL_DestroyTexture(dino->texture[3]);
    SDL_DestroyTexture(dino->texture[4]);
}