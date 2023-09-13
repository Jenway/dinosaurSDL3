#include "SDL_rect.h"
#if !defined(_REX_H_)
#define _REX_H_
#include "lTexture.h"
#include <SDL_render.h>
#include <stdbool.h>

// position of the trex in the sprite
#define TREX_POS_1_X 0
#define TREX_POS_1_Y 0
#define TREX_POS_2_WIDTH 44
#define TREX_POS_2_HEIGHT 47

typedef struct trex {

    LTexture* texture;
    SDL_Rect spritePos;
    int xPos;
    int yPos;
    int xInitialPos;

    // position of the trex in the sprite
    SDL_Point pos[6];

    // Position when on the ground.
    int groundYPos;
    SDL_Texture* currentFrame;
    SDL_Texture* currentAnimFrames[5];

    int blinkDelay;
    int blinkCount;
    int animStartTime;
    int timer;
    int msPerFrame;

    int status;
    bool jumping;
    bool ducking;
    int jumpVelocity;
    int minJumpHeight;
    bool reachedMinHeight;
    bool speedDrop;
    int jumpCount;
    int jumpspotX;
    bool altGameModeEnabled;
    bool flashing;
    bool playingIntro;
    bool midair;
    bool INVERT_JUMP;

    // config
    int DROP_VELOCITY;
    int FLASH_OFF;
    int FLASH_ON;
    int HEIGHT;
    int HEIGHT_DUCK;
    int INTRO_DURATION;
    int SPEED_DROP_COEFFICIENT;
    int SPRITE_WIDTH;
    int START_X_POS;
    int WIDTH;
    int WIDTH_DUCK;
    int WIDTH_JUMP;
    int BLINK_TIMING;

    // jump config
    float GRAVITY;
    int MAX_JUMP_HEIGHT;
    int MIN_JUMP_HEIGHT;
    int INITIAL_JUMP_VELOCITY;

} Trex;

Trex* Trex_constructor();
void init(Trex* self);
void enableSlowConfig(Trex* this);
void setFlashing(Trex* this, bool status);
void setJumpVelocity(Trex* this, int setting);
void update(Trex* this, int deltaTime, int opt_status);
void draw(Trex* this, int x, int y);
void setBlinkDelay(Trex* this);
void blink(Trex* this, int time);
void startJump(Trex* this, int speed);
void endJump(Trex* this);
void updateJump(Trex* this, int deltaTime);
void setSpeedDrop(Trex* this);
void setDuck(Trex* this, bool isDucking);
void reset(Trex* this);

#endif // _REX_H_
