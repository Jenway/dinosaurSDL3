#if !defined(_DATA_H_)
#define _DATA_H_

#define SCREEN_TITLE "Chrome Dino"

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 300
// the width : height ratio of the screen should be 4 : 1

#define FPS 60
#define FRAME_TIME (1000 / FPS)

#define SPRITE_PATH "assets/sprite.png"

#define SPRITE_WIDTH 1200
#define SPRITE_HEIGHT 300

#define GRAVITY 0.6

/*
 * sprite definitions
 */

#define SPRITE_TREX_WIDTH 88
#define SPRITE_TREX_HEIGHT 94
#define SPRITE_TREX_X_BEGIN 1678
#define SPRITE_TREX_Y 2

#define SPRITE_TREX_WATING_STEAY \
    {                            \
        .x = 76,                 \
        .y = 6,                  \
        .w = SPRITE_TREX_WIDTH,  \
        .h = SPRITE_TREX_HEIGHT  \
    }

#define SPRITE_TREX_WATING_1                          \
    {                                                 \
        .x = SPRITE_TREX_X_BEGIN + SPRITE_TREX_WIDTH, \
        .y = SPRITE_TREX_Y,                           \
        .w = SPRITE_TREX_WIDTH,                       \
        .h = SPRITE_TREX_HEIGHT                       \
    }

#define SPRITE_TREX_WATING_2      \
    {                             \
        .x = SPRITE_TREX_X_BEGIN, \
        .y = SPRITE_TREX_Y,       \
        .w = SPRITE_TREX_WIDTH,   \
        .h = SPRITE_TREX_HEIGHT   \
    }

#define SPRITE_TREX_RUNNING_1   \
    {                           \
        .x = 1854,              \
        .y = SPRITE_TREX_Y,     \
        .w = SPRITE_TREX_WIDTH, \
        .h = SPRITE_TREX_HEIGHT \
    }

#define SPRITE_TREX_RUNNING_2                             \
    {                                                     \
        .x = SPRITE_TREX_X_BEGIN + 3 * SPRITE_TREX_WIDTH, \
        .y = SPRITE_TREX_Y,                               \
        .w = SPRITE_TREX_WIDTH,                           \
        .h = SPRITE_TREX_HEIGHT                           \
    }

#define SPRITE_TREX_CRASHED_1                             \
    {                                                     \
        .x = SPRITE_TREX_X_BEGIN + 4 * SPRITE_TREX_WIDTH, \
        .y = SPRITE_TREX_Y,                               \
        .w = SPRITE_TREX_WIDTH,                           \
        .h = SPRITE_TREX_HEIGHT                           \
    }

#define SPRITE_TREX_CRASHED_2                             \
    {                                                     \
        .x = SPRITE_TREX_X_BEGIN + 5 * SPRITE_TREX_WIDTH, \
        .y = SPRITE_TREX_Y,                               \
        .w = SPRITE_TREX_WIDTH,                           \
        .h = SPRITE_TREX_HEIGHT                           \
    }

#define SPRITE_TREX_DUCKING_1 \
    {                         \
        .x = 2206,            \
        .y = 36,              \
        .w = 118,             \
        .h = 60               \
    }

#define SPRITE_TREX_DUCKING_2                             \
    {                                                     \
        .x = SPRITE_TREX_X_BEGIN + 7 * SPRITE_TREX_WIDTH, \
        .y = 36,                                          \
        .w = 118,                                         \
        .h = 60                                           \
    }

/*
 * END OF TREX SPRITE DEFINITIONS
 */

#define SPRITE_BACKGROUND_CLOUD            \
    {                                      \
        .x = 166, .y = 2, .w = 46, .h = 28 \
    }

#define SPRITE_CACTUS_LARGE

#endif // _DATA_H_