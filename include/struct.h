#if !defined(__STRUCT_H__)
#define __STRUCT_H__
#define _REX_H_
// TREX

#if defined(_REX_H_)
struct Config {
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
};

struct JumpConfig {
    float GRAVITY;
    int MAX_JUMP_HEIGHT;
    int MIN_JUMP_HEIGHT;
    int INITIAL_JUMP_VELOCITY;
};

#endif

#endif // __STRUCT_H__