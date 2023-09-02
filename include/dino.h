#if !defined(_DINO_H_)
#define _DINO_H_

#include <SDL_render.h>
#include <SDL_events.h>
#include <stdbool.h>
#include "lTexture.h"

enum DINO_FRAME {
    
    DINO_FRAME_CRASHED,
    DINO_FRAME_DUCKING,
    DINO_FRAME_JUMPING,
    DINO_FRAME_RUNNING,
    DINO_FRAME_WAITING,
    DINO_FRAME_COUNT
};

typedef struct dino {
    bool Alive;
    bool OnAir;
    bool OnCrawl;
    bool jumping;
    bool ducking;
    int y_velocity;
    SDL_Rect position;
    // collision box
    SDL_Rect m_collider;
    // frame
    int frame;
    enum DINO_FRAME currentFrame;
    // dino frame timer
    Uint32 frame_timer;

    // Texture
    SDL_Texture* texture[DINO_FRAME_COUNT];
    // LTexture* texture[DINO_FRAME_COUNT];
    bool speedDrop;
    bool reachedMinHeight;
} Dino;

Dino* Dino_constructor();
void Dino_destructor(Dino* this);

void Dino_Init(Dino *dino);
bool Dino_Load(Dino *dino,SDL_Renderer* renderer);
void Dino_Update(Dino *dino);
void Dino_Draw(Dino *dino,SDL_Renderer* renderer);
void Dino_Destroy(Dino *dino);
void Dino_handleEvent(Dino *dino,SDL_Event* e,bool *quit);
void Dino_Update_position(Dino *dino);
void Dino_Update_Frame(Dino *dino);
SDL_Rect Dino_Get_Collision(Dino *dino);
bool Dino_Alive(Dino *dino);

void Dino_Crawl(Dino* dino);
void Dino_DeCrawl(Dino* dino);


// Dino Jump

void Dino_Jump(Dino *dino);
void Dino_StartJump(Dino* this, int speed);
void Dino_EndJump(Dino* this);
void Dino_UpdateJump(Dino* this, int deltaTime);

/**
 * Set the speed drop. Immediately cancels the current jump.
 */
void Dino_SetSpeedDrop(Dino* this);

// Dino Duck

void Dino_SetDuck(Dino* this, bool isDucking);


/**
 * Reset the t-rex to running at start of game.
 */
void Dino_Reset(Dino* this);
#endif // _DINO_H_