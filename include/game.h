#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "obstacles.h"

#define GAME_WORLD_WIDTH 800
#define GAME_WORLD_HEIGHT 300
#define DINO_FOOT_HEIGHT 94
#define DINO_WORLD_X 47
#define DINO_WORLD_Y (GAME_WORLD_HEIGHT + DINO_FOOT_HEIGHT)
#define MAX_OBSTACLES 8
#define MAX_CLOUDS 6
#define MAX_DINO_COLLISION_BOXES 6
#define GAME_SPRITE_SCALE 2
#define ROAD_TILE_WIDTH 2400
#define NIGHT_STAR_COUNT 2

typedef enum dino_status
{
    DINO_STATUS_IDLE = 0,
    DINO_STATUS_RUNNING = 1,
    DINO_STATUS_JUMPING = 2,
    DINO_STATUS_SITTING = 3,
    DINO_STATUS_DEAD = 4,
} dino_status;

typedef enum game_phase
{
    GAME_PHASE_START = 0,
    GAME_PHASE_PLAYING = 1,
    GAME_PHASE_GAME_OVER = 2,
} game_phase;

typedef struct dino_state
{
    float jump_height;
    float vertical_velocity;
    dino_status status;
    int running_frame;
    int animation_ticks;
    bool jump_release_ready;
    bool fast_fall_active;
} dino_state;

typedef struct cloud_state
{
    uint32_t id;
    float x;
    int y;
    float gap;
} cloud_state;

typedef struct night_star_state
{
    float x;
    int y;
    int variant;
} night_star_state;

typedef struct world_state
{
    float road_scroll_offset;
    cloud_state clouds[MAX_CLOUDS];
    size_t cloud_count;
    uint32_t next_cloud_id;
    bool night_active;
    float night_blend;
    float moon_x;
    int moon_phase;
    uint32_t next_night_mode_score;
    night_star_state stars[NIGHT_STAR_COUNT];
} world_state;

typedef struct score_state
{
    float accumulator;
    uint32_t current;
    uint32_t high;
    bool achievement_active;
    int flash_ticks;
    int flash_iterations;
    bool high_score_dirty;
} score_state;

typedef struct obstacle_state
{
    uint32_t id;
    obstacle_type type;
    float x;
    int y;
    float speed_offset;
    int animation_frame;
    double animation_elapsed_ms;
} obstacle_state;

typedef struct obstacle_system_state
{
    obstacle_state items[MAX_OBSTACLES];
    size_t count;
    float spawn_distance;
    uint32_t next_id;
} obstacle_system_state;

typedef struct runtime_state
{
    double tick_ms;
    double accumulator_ms;
    double last_tick_ms;
    int scroll_speed;
    bool duck_held;
    bool awaiting_start;
} runtime_state;

typedef struct game_state
{
    dino_state dino;
    world_state world;
    score_state score;
    obstacle_system_state obstacles;
    runtime_state runtime;
} game_state;

void game_state_init(game_state *game);
void game_restart(game_state *game);
void game_handle_jump_press(game_state *game);
void game_handle_jump_release(game_state *game);
void game_set_duck_held(game_state *game, bool held);
void game_set_high_score(game_state *game, uint32_t high_score);
bool game_consume_high_score_dirty(game_state *game, uint32_t *high_score);
void game_update_step(game_state *game);
game_phase game_get_phase(const game_state *game);
bool game_is_playing(const game_state *game);
bool game_should_draw_score(const game_state *game);
size_t game_collect_dino_collision_boxes(float jump_height,
                                         dino_status status,
                                         collision_box *boxes,
                                         size_t max_boxes);

#endif
