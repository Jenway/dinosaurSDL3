#include "game.h"

#include <stdbool.h>
#include <stdlib.h>

typedef struct game_rect
{
    int x;
    int y;
    int w;
    int h;
} game_rect;

typedef struct gameplay_tuning
{
    double tick_ms;
    float scroll_scale;
    int base_scroll_speed;
    int max_scroll_speed;
    int score_speed_step;
    uint32_t milestone_score_interval;
    int score_flash_half_cycle_ticks;
    int score_flash_iterations;
} gameplay_tuning;

typedef struct dino_tuning
{
    int animation_frame_ticks;
    float start_jump_height;
    float min_jump_height;
    float max_jump_height;
    float initial_jump_velocity;
    float gravity;
    float drop_velocity;
    float speed_drop_velocity;
    float speed_drop_coefficient;
    float jump_speed_factor;
} dino_tuning;

typedef struct world_tuning
{
    int initial_obstacle_spawn_distance;
    int obstacle_spawn_padding;
    float obstacle_gap_coefficient;
    int obstacle_gap_speed_divisor;
    int min_gap_speed;
    int max_gap_speed;
    int cloud_sprite_width;
    int cloud_min_gap;
    int cloud_max_gap;
    int cloud_min_y;
    int cloud_max_y;
    float cloud_speed_factor;
    uint32_t night_mode_score_interval;
    float night_fade_step;
    float moon_scroll_speed_divisor;
    float star_scroll_speed_divisor;
    int moon_entry_offset;
    int moon_wrap_padding;
    int star_min_y;
    int star_max_y;
    int star_wrap_padding;
} world_tuning;

typedef struct background_scroll_config
{
    float speed_factor;
    float wrap_min_x;
    float wrap_max_x;
} background_scroll_config;

static const gameplay_tuning gameplay_config = {
    .tick_ms = 1000.0 / 60.0,
    .scroll_scale = 0.5f,
    .base_scroll_speed = 15,
    .max_scroll_speed = 30,
    .score_speed_step = 150,
    .milestone_score_interval = 100,
    .score_flash_half_cycle_ticks = 15,
    .score_flash_iterations = 3,
};

static const dino_tuning dino_config = {
    .animation_frame_ticks = 5,
    .start_jump_height = 200.0f,
    .min_jump_height = 260.0f,
    .max_jump_height = 330.0f,
    .initial_jump_velocity = 19.0f,
    .gravity = 1.2f,
    .drop_velocity = 9.0f,
    .speed_drop_velocity = -1.0f,
    .speed_drop_coefficient = 3.0f,
    .jump_speed_factor = 0.1f,
};

static const world_tuning world_config = {
    .initial_obstacle_spawn_distance = 360,
    .obstacle_spawn_padding = 48,
    .obstacle_gap_coefficient = 0.6f,
    .obstacle_gap_speed_divisor = 3,
    .min_gap_speed = 6,
    .max_gap_speed = 11,
    .cloud_sprite_width = 92,
    .cloud_min_gap = 200,
    .cloud_max_gap = 800,
    .cloud_min_y = 60,
    .cloud_max_y = 142,
    .cloud_speed_factor = 1.0f / 12.0f,
    .night_mode_score_interval = 700,
    .night_fade_step = 0.035f,
    .moon_scroll_speed_divisor = 4.0f,
    .star_scroll_speed_divisor = 7.0f,
    .moon_entry_offset = 80,
    .moon_wrap_padding = 120,
    .star_min_y = 20,
    .star_max_y = 110,
    .star_wrap_padding = 140,
};

static const background_scroll_config road_scroll_config = {
    .speed_factor = 1.0f,
    .wrap_min_x = 0.0f,
    .wrap_max_x = (float)ROAD_TILE_WIDTH,
};

static const collision_box dino_running_collision_boxes[] = {
    {22, 0, 17, 16},
    {1, 18, 30, 9},
    {10, 35, 14, 8},
    {1, 24, 29, 5},
    {5, 30, 21, 4},
    {9, 34, 15, 4},
};

static const collision_box dino_ducking_collision_boxes[] = {
    {1, 18, 55, 25},
};

static float gameplay_scale_speed(float speed)
{
    return speed * gameplay_config.scroll_scale;
}

static int gameplay_random_range(int min_value, int max_value)
{
    int span = max_value - min_value + 1;

    if (span <= 1)
    {
        return min_value;
    }

    return min_value + rand() % span;
}

static bool dino_is_airborne(const dino_state *dino)
{
    return dino->jump_height > dino_config.start_jump_height;
}

static void dino_reset(dino_state *dino)
{
    dino->jump_height = dino_config.start_jump_height;
    dino->vertical_velocity = 0.0f;
    dino->status = DINO_STATUS_IDLE;
    dino->running_frame = 0;
    dino->animation_ticks = 0;
    dino->jump_release_ready = false;
    dino->fast_fall_active = false;
}

static float world_random_cloud_gap(void)
{
    return (float)gameplay_random_range(world_config.cloud_min_gap, world_config.cloud_max_gap);
}

static float world_wrap_position(float x, float min_x, float max_x)
{
    float span = max_x - min_x;

    if (span <= 0.0f)
    {
        return min_x;
    }

    while (x < min_x)
    {
        x += span;
    }

    while (x >= max_x)
    {
        x -= span;
    }

    return x;
}

static void world_advance_wrapped_position(float *x, float delta, background_scroll_config config)
{
    float next_x = *x + delta * config.speed_factor;

    *x = world_wrap_position(next_x, config.wrap_min_x, config.wrap_max_x);
}

static background_scroll_config world_make_moon_scroll_config(void)
{
    return (background_scroll_config){
        .speed_factor = 1.0f / world_config.moon_scroll_speed_divisor,
        .wrap_min_x = -(float)world_config.moon_wrap_padding,
        .wrap_max_x = (float)(GAME_WORLD_WIDTH + world_config.moon_wrap_padding),
    };
}

static void world_place_stars(world_state *world)
{
    int segment_width = GAME_WORLD_WIDTH / NIGHT_STAR_COUNT;

    for (int i = 0; i < NIGHT_STAR_COUNT; i++)
    {
        world->stars[i].x = (float)(GAME_WORLD_WIDTH + i * segment_width + gameplay_random_range(0, segment_width));
        world->stars[i].y = gameplay_random_range(world_config.star_min_y, world_config.star_max_y);
        world->stars[i].variant = gameplay_random_range(0, 1);
    }
}

static void world_respawn_star(world_state *world, int index, float x)
{
    world->stars[index].x = x;
    world->stars[index].y = gameplay_random_range(world_config.star_min_y, world_config.star_max_y);
    world->stars[index].variant = gameplay_random_range(0, 1);
}

static void world_spawn_cloud(world_state *world, float x)
{
    cloud_state *cloud;

    if (world->cloud_count >= MAX_CLOUDS)
    {
        return;
    }

    cloud = &world->clouds[world->cloud_count];
    cloud->id = world->next_cloud_id;
    world->next_cloud_id += 1;
    cloud->x = x;
    cloud->y = gameplay_random_range(world_config.cloud_min_y, world_config.cloud_max_y);
    cloud->gap = world_random_cloud_gap();
    world->cloud_count += 1;
}

static const cloud_state *world_rightmost_cloud(const world_state *world)
{
    const cloud_state *rightmost = NULL;

    for (size_t i = 0; i < world->cloud_count; i++)
    {
        if (rightmost == NULL || world->clouds[i].x > rightmost->x)
        {
            rightmost = &world->clouds[i];
        }
    }

    return rightmost;
}

static void world_seed_clouds(world_state *world)
{
    float x = 120.0f;

    for (int i = 0; i < 3; i++)
    {
        world_spawn_cloud(world, x);
        x += (float)world_config.cloud_sprite_width + world->clouds[world->cloud_count - 1].gap;
    }
}

static void world_reset(world_state *world)
{
    world->road_scroll_offset = 0.0f;
    world->cloud_count = 0;
    world->next_cloud_id = 1;
    world_seed_clouds(world);
    world->night_active = false;
    world->night_blend = 0.0f;
    world->moon_x = (float)(GAME_WORLD_WIDTH - world_config.moon_entry_offset);
    world->moon_phase = 0;
    world->next_night_mode_score = world_config.night_mode_score_interval;
    world_place_stars(world);
}

static void score_reset(score_state *score, uint32_t high_score)
{
    score->accumulator = 0.0f;
    score->current = 0;
    score->high = high_score;
    score->achievement_active = false;
    score->flash_ticks = 0;
    score->flash_iterations = 0;
    score->high_score_dirty = false;
}

static void obstacle_system_reset(obstacle_system_state *obstacles)
{
    obstacles->count = 0;
    obstacles->spawn_distance = (float)world_config.initial_obstacle_spawn_distance;
    obstacles->next_id = 1;
}

static void runtime_reset(runtime_state *runtime, bool awaiting_start)
{
    runtime->tick_ms = gameplay_config.tick_ms;
    runtime->accumulator_ms = 0.0;
    runtime->last_tick_ms = 0.0;
    runtime->scroll_speed = gameplay_config.base_scroll_speed;
    runtime->duck_held = false;
    runtime->awaiting_start = awaiting_start;
}

static void game_reset(game_state *game, bool awaiting_start)
{
    uint32_t high_score = game->score.high;

    dino_reset(&game->dino);
    world_reset(&game->world);
    score_reset(&game->score, high_score);
    obstacle_system_reset(&game->obstacles);
    runtime_reset(&game->runtime, awaiting_start);
}

static bool collision_intersects(game_rect a, game_rect b)
{
    return a.x < (b.x + b.w) && (a.x + a.w) > b.x && a.y < (b.y + b.h) && (a.y + a.h) > b.y;
}

static game_rect collision_box_to_rect(int base_x, int base_y, collision_box box)
{
    return (game_rect){
        .x = base_x + box.x * GAME_SPRITE_SCALE,
        .y = base_y + box.y * GAME_SPRITE_SCALE,
        .w = box.width * GAME_SPRITE_SCALE,
        .h = box.height * GAME_SPRITE_SCALE,
    };
}

static size_t collision_collect_dino_boxes(const dino_state *dino, game_rect *boxes)
{
    int base_y = (int)((float)DINO_WORLD_Y - dino->jump_height);

    if (dino->status == DINO_STATUS_SITTING)
    {
        boxes[0] = collision_box_to_rect(DINO_WORLD_X, base_y, dino_ducking_collision_boxes[0]);
        return sizeof(dino_ducking_collision_boxes) / sizeof(dino_ducking_collision_boxes[0]);
    }

    for (size_t i = 0; i < sizeof(dino_running_collision_boxes) / sizeof(dino_running_collision_boxes[0]); i++)
    {
        boxes[i] = collision_box_to_rect(DINO_WORLD_X, base_y, dino_running_collision_boxes[i]);
    }

    return sizeof(dino_running_collision_boxes) / sizeof(dino_running_collision_boxes[0]);
}

static size_t collision_collect_obstacle_boxes(const obstacle_state *obstacle, game_rect *boxes)
{
    const obstacle_definition *definition = obstacle_get_definition(obstacle->type);
    int base_x = (int)obstacle->x;

    for (size_t i = 0; i < definition->collision_box_count; i++)
    {
        boxes[i] = collision_box_to_rect(base_x, obstacle->y, definition->collision_boxes[i]);
    }

    return definition->collision_box_count;
}

static void collision_step(game_state *game)
{
    game_rect dino_boxes[sizeof(dino_running_collision_boxes) / sizeof(dino_running_collision_boxes[0])];
    size_t dino_box_count;

    if (game->dino.status == DINO_STATUS_DEAD || game->runtime.awaiting_start)
    {
        return;
    }

    dino_box_count = collision_collect_dino_boxes(&game->dino, dino_boxes);
    for (size_t i = 0; i < game->obstacles.count; i++)
    {
        game_rect obstacle_boxes[MAX_OBSTACLE_COLLISION_BOXES];
        size_t obstacle_box_count = collision_collect_obstacle_boxes(&game->obstacles.items[i], obstacle_boxes);

        for (size_t dino_index = 0; dino_index < dino_box_count; dino_index++)
        {
            for (size_t obstacle_index = 0; obstacle_index < obstacle_box_count; obstacle_index++)
            {
                if (collision_intersects(dino_boxes[dino_index], obstacle_boxes[obstacle_index]))
                {
                    game->dino.status = DINO_STATUS_DEAD;
                    return;
                }
            }
        }
    }
}

static void dino_step_animation(dino_state *dino)
{
    if (dino->status == DINO_STATUS_RUNNING || dino->status == DINO_STATUS_SITTING)
    {
        if (dino->animation_ticks >= dino_config.animation_frame_ticks)
        {
            dino->running_frame = (dino->running_frame + 1) % 2;
            dino->animation_ticks = 0;
        }
        else
        {
            dino->animation_ticks++;
        }
    }
    else
    {
        dino->animation_ticks = 0;
        dino->running_frame = 0;
    }
}

static void dino_begin_jump(dino_state *dino, int scroll_speed)
{
    dino->status = DINO_STATUS_JUMPING;
    dino->vertical_velocity = dino_config.initial_jump_velocity - (float)scroll_speed * dino_config.jump_speed_factor;
    dino->jump_release_ready = false;
    dino->fast_fall_active = false;
}

static void dino_end_jump(dino_state *dino)
{
    if (dino->jump_release_ready && dino->vertical_velocity > dino_config.drop_velocity)
    {
        dino->vertical_velocity = dino_config.drop_velocity;
    }
}

static void dino_begin_fast_fall(dino_state *dino)
{
    dino->jump_release_ready = true;
    dino->fast_fall_active = true;
    dino->vertical_velocity = dino_config.speed_drop_velocity;
}

static void dino_step_vertical_motion(dino_state *dino)
{
    float movement = dino->vertical_velocity;

    if (dino->fast_fall_active)
    {
        movement *= dino_config.speed_drop_coefficient;
    }

    dino->jump_height += movement;
    dino->vertical_velocity -= dino_config.gravity;

    if (dino->jump_height >= dino_config.min_jump_height)
    {
        dino->jump_release_ready = true;
    }

    if (dino->jump_height >= dino_config.max_jump_height || dino->fast_fall_active)
    {
        dino_end_jump(dino);
    }

    if (dino->jump_height <= dino_config.start_jump_height)
    {
        dino->jump_height = dino_config.start_jump_height;
        dino->vertical_velocity = 0.0f;
        dino->jump_release_ready = false;
        dino->fast_fall_active = false;
    }
}

static void dino_sync_ground_state(game_state *game)
{
    dino_state *dino = &game->dino;

    if (game->runtime.awaiting_start || dino->status == DINO_STATUS_DEAD)
    {
        return;
    }

    if (dino->status == DINO_STATUS_JUMPING && dino_is_airborne(dino))
    {
        if (game->runtime.duck_held && !dino->fast_fall_active)
        {
            dino_begin_fast_fall(dino);
        }
        return;
    }

    if (dino_is_airborne(dino))
    {
        return;
    }

    dino->status = game->runtime.duck_held ? DINO_STATUS_SITTING : DINO_STATUS_RUNNING;
}

static float obstacle_random_speed_offset(float base_offset)
{
    if (base_offset == 0.0f)
    {
        return 0.0f;
    }

    return gameplay_random_range(0, 1) == 0 ? -base_offset : base_offset;
}

static int obstacle_system_gap_speed(int scroll_speed)
{
    int gap_speed = scroll_speed / world_config.obstacle_gap_speed_divisor + 1;

    if (gap_speed < world_config.min_gap_speed)
    {
        return world_config.min_gap_speed;
    }

    if (gap_speed > world_config.max_gap_speed)
    {
        return world_config.max_gap_speed;
    }

    return gap_speed;
}

static float obstacle_system_next_spawn_distance(const obstacle_definition *definition, int scroll_speed)
{
    int gap_speed = obstacle_system_gap_speed(scroll_speed);
    float min_gap = definition->sprite_width * (float)gap_speed + definition->min_gap * world_config.obstacle_gap_coefficient;
    float max_gap = min_gap * 1.5f;

    return (float)gameplay_random_range((int)min_gap, (int)max_gap);
}

static obstacle_type obstacle_system_choose_type(int scroll_speed)
{
    obstacle_type eligible[OBSTACLE_TYPE_COUNT];
    size_t eligible_count = 0;

    for (int i = 0; i < (int)obstacle_definition_count(); i++)
    {
        obstacle_type type = (obstacle_type)i;
        const obstacle_definition *definition = obstacle_get_definition(type);

        if (scroll_speed >= definition->min_scroll_speed)
        {
            eligible[eligible_count] = type;
            eligible_count += 1;
        }
    }

    return eligible[gameplay_random_range(0, (int)eligible_count - 1)];
}

static void obstacle_system_spawn(obstacle_system_state *obstacles, int scroll_speed)
{
    const obstacle_definition *definition;
    obstacle_state *obstacle;
    obstacle_type type;

    if (obstacles->count >= MAX_OBSTACLES)
    {
        return;
    }

    type = obstacle_system_choose_type(scroll_speed);
    definition = obstacle_get_definition(type);
    obstacle = &obstacles->items[obstacles->count];
    obstacle->id = obstacles->next_id;
    obstacles->next_id += 1;
    obstacle->type = type;
    obstacle->x = (float)(GAME_WORLD_WIDTH + world_config.obstacle_spawn_padding);
    obstacle->y = definition->y_positions[gameplay_random_range(0, (int)definition->y_position_count - 1)];
    obstacle->speed_offset = obstacle_random_speed_offset(definition->speed_offset);
    obstacle->animation_frame = 0;
    obstacle->animation_elapsed_ms = 0.0;
    obstacles->count += 1;
    obstacles->spawn_distance = obstacle_system_next_spawn_distance(definition, scroll_speed);
}

static void obstacle_step_animation(obstacle_state *obstacle, const obstacle_definition *definition, double tick_ms)
{
    if (definition->num_frames <= 1 || definition->frame_ms <= 0)
    {
        return;
    }

    obstacle->animation_elapsed_ms += tick_ms;
    while (obstacle->animation_elapsed_ms >= (double)definition->frame_ms)
    {
        obstacle->animation_elapsed_ms -= (double)definition->frame_ms;
        obstacle->animation_frame = (obstacle->animation_frame + 1) % definition->num_frames;
    }
}

static void obstacle_system_step(obstacle_system_state *obstacles, int scroll_speed, dino_status status, double tick_ms)
{
    float scroll_step = gameplay_scale_speed((float)scroll_speed);
    size_t write_index = 0;

    if (status == DINO_STATUS_DEAD)
    {
        return;
    }

    for (size_t i = 0; i < obstacles->count; i++)
    {
        obstacle_state obstacle = obstacles->items[i];
        const obstacle_definition *definition = obstacle_get_definition(obstacle.type);

        obstacle.x -= gameplay_scale_speed((float)scroll_speed + obstacle.speed_offset);
        obstacle_step_animation(&obstacle, definition, tick_ms);
        if ((int)obstacle.x + definition->sprite_width > 0)
        {
            obstacles->items[write_index] = obstacle;
            write_index += 1;
        }
    }
    obstacles->count = write_index;

    obstacles->spawn_distance -= scroll_step;
    if (obstacles->spawn_distance <= 0.0f)
    {
        obstacle_system_spawn(obstacles, scroll_speed);
    }
}

static void world_toggle_night_mode(world_state *world)
{
    world->night_active = !world->night_active;
    if (world->night_active)
    {
        world->moon_phase = (world->moon_phase + 1) % 7;
        world->moon_x = (float)(GAME_WORLD_WIDTH - world_config.moon_entry_offset);
        world_place_stars(world);
    }
}

static void score_trigger_achievement(score_state *score)
{
    score->achievement_active = true;
    score->flash_ticks = 0;
    score->flash_iterations = 0;
}

static void score_step(game_state *game)
{
    score_state *score = &game->score;
    runtime_state *runtime = &game->runtime;
    world_state *world = &game->world;
    int next_speed;

    if (game->dino.status == DINO_STATUS_DEAD || runtime->awaiting_start)
    {
        return;
    }

    score->accumulator += gameplay_config.scroll_scale;
    while (score->accumulator >= 1.0f)
    {
        score->current += 1;
        score->accumulator -= 1.0f;

        if (score->current > 0 && score->current % gameplay_config.milestone_score_interval == 0)
        {
            score_trigger_achievement(score);
        }

        if (score->current > score->high)
        {
            score->high = score->current;
            score->high_score_dirty = true;
        }

        if (score->current >= world->next_night_mode_score)
        {
            world_toggle_night_mode(world);
            world->next_night_mode_score += world_config.night_mode_score_interval;
        }
    }

    next_speed = gameplay_config.base_scroll_speed + (int)(score->current / gameplay_config.score_speed_step);
    if (next_speed > gameplay_config.max_scroll_speed)
    {
        next_speed = gameplay_config.max_scroll_speed;
    }
    runtime->scroll_speed = next_speed;
}

static void score_step_feedback(score_state *score)
{
    int full_cycle_ticks = gameplay_config.score_flash_half_cycle_ticks * 2;

    if (!score->achievement_active)
    {
        return;
    }

    score->flash_ticks += 1;
    if (score->flash_ticks >= full_cycle_ticks)
    {
        score->flash_ticks = 0;
        score->flash_iterations += 1;
        if (score->flash_iterations >= gameplay_config.score_flash_iterations)
        {
            score->achievement_active = false;
            score->flash_ticks = 0;
            score->flash_iterations = 0;
        }
    }
}

static float world_cloud_scroll_step(float scroll_step)
{
    return scroll_step * world_config.cloud_speed_factor;
}

static void world_step_clouds(world_state *world, float scroll_step)
{
    float cloud_step = world_cloud_scroll_step(scroll_step);
    size_t write_index = 0;
    const cloud_state *rightmost;

    for (size_t i = 0; i < world->cloud_count; i++)
    {
        cloud_state cloud = world->clouds[i];

        cloud.x -= cloud_step;
        if (cloud.x + (float)world_config.cloud_sprite_width > -40.0f)
        {
            world->clouds[write_index] = cloud;
            write_index += 1;
        }
    }
    world->cloud_count = write_index;

    if (world->cloud_count == 0)
    {
        world_spawn_cloud(world, (float)GAME_WORLD_WIDTH);
        return;
    }

    rightmost = world_rightmost_cloud(world);
    if (rightmost != NULL &&
        world->cloud_count < MAX_CLOUDS &&
        rightmost->x + (float)world_config.cloud_sprite_width + rightmost->gap <= (float)GAME_WORLD_WIDTH)
    {
        world_spawn_cloud(world, (float)GAME_WORLD_WIDTH + (float)gameplay_random_range(0, 60));
    }
}

static void world_step_stars(world_state *world, float scroll_step)
{
    for (int i = 0; i < NIGHT_STAR_COUNT; i++)
    {
        float star_speed = scroll_step / world_config.star_scroll_speed_divisor;

        world->stars[i].x -= star_speed;
        if (world->stars[i].x < (float)(-world_config.star_wrap_padding))
        {
            world_respawn_star(world,
                               i,
                               (float)(GAME_WORLD_WIDTH + gameplay_random_range(0, world_config.star_wrap_padding)));
        }
    }
}

static void world_step_night_sky(world_state *world, float scroll_step)
{
    background_scroll_config moon_scroll_config = world_make_moon_scroll_config();

    if (world->night_active)
    {
        if (world->night_blend < 1.0f)
        {
            world->night_blend += world_config.night_fade_step;
            if (world->night_blend > 1.0f)
            {
                world->night_blend = 1.0f;
            }
        }
    }
    else if (world->night_blend > 0.0f)
    {
        world->night_blend -= world_config.night_fade_step;
        if (world->night_blend < 0.0f)
        {
            world->night_blend = 0.0f;
        }
    }

    if (world->night_active || world->night_blend > 0.0f)
    {
        world_advance_wrapped_position(&world->moon_x, -scroll_step, moon_scroll_config);
        world_step_stars(world, scroll_step);
    }
}

static void world_step(world_state *world, int scroll_speed, dino_status status)
{
    float scroll_step;

    if (status == DINO_STATUS_DEAD)
    {
        return;
    }

    scroll_step = gameplay_scale_speed((float)scroll_speed);
    world_advance_wrapped_position(&world->road_scroll_offset, scroll_step, road_scroll_config);
    world_step_clouds(world, scroll_step);
    world_step_night_sky(world, scroll_step);
}

void game_state_init(game_state *game)
{
    game->score.high = 0;
    game_reset(game, true);
}

void game_restart(game_state *game)
{
    game_reset(game, false);
}

void game_handle_jump_press(game_state *game)
{
    if (game->runtime.awaiting_start)
    {
        game->runtime.awaiting_start = false;
        dino_begin_jump(&game->dino, game->runtime.scroll_speed);
        return;
    }

    if (game->dino.status == DINO_STATUS_DEAD)
    {
        game_restart(game);
        dino_begin_jump(&game->dino, game->runtime.scroll_speed);
        return;
    }

    if (game->dino.status == DINO_STATUS_RUNNING || game->dino.status == DINO_STATUS_SITTING)
    {
        dino_begin_jump(&game->dino, game->runtime.scroll_speed);
    }
}

void game_handle_jump_release(game_state *game)
{
    if (game->dino.status == DINO_STATUS_JUMPING)
    {
        dino_end_jump(&game->dino);
    }
}

void game_set_duck_held(game_state *game, bool held)
{
    game->runtime.duck_held = held;
    if (!held)
    {
        game->dino.fast_fall_active = false;
    }
}

void game_set_high_score(game_state *game, uint32_t high_score)
{
    if (high_score > game->score.high)
    {
        game->score.high = high_score;
        game->score.high_score_dirty = false;
    }
}

bool game_consume_high_score_dirty(game_state *game, uint32_t *high_score)
{
    if (!game->score.high_score_dirty)
    {
        return false;
    }

    game->score.high_score_dirty = false;
    *high_score = game->score.high;
    return true;
}

size_t game_collect_dino_collision_boxes(float jump_height,
                                         dino_status status,
                                         collision_box *boxes,
                                         size_t max_boxes)
{
    const collision_box *source_boxes = dino_running_collision_boxes;
    size_t source_count = sizeof(dino_running_collision_boxes) / sizeof(dino_running_collision_boxes[0]);
    int base_y = (int)((float)DINO_WORLD_Y - jump_height);

    if (status == DINO_STATUS_SITTING)
    {
        source_boxes = dino_ducking_collision_boxes;
        source_count = sizeof(dino_ducking_collision_boxes) / sizeof(dino_ducking_collision_boxes[0]);
    }

    if (source_count > max_boxes)
    {
        source_count = max_boxes;
    }

    for (size_t i = 0; i < source_count; i++)
    {
        boxes[i] = (collision_box){
            .x = DINO_WORLD_X + source_boxes[i].x * GAME_SPRITE_SCALE,
            .y = base_y + source_boxes[i].y * GAME_SPRITE_SCALE,
            .width = source_boxes[i].width * GAME_SPRITE_SCALE,
            .height = source_boxes[i].height * GAME_SPRITE_SCALE,
        };
    }

    return source_count;
}

void game_update_step(game_state *game)
{
    if (game->dino.status == DINO_STATUS_JUMPING)
    {
        dino_step_vertical_motion(&game->dino);
    }

    dino_sync_ground_state(game);
    dino_step_animation(&game->dino);
    score_step(game);
    score_step_feedback(&game->score);
    world_step(&game->world, game->runtime.scroll_speed, game->dino.status);
    obstacle_system_step(&game->obstacles, game->runtime.scroll_speed, game->dino.status, game->runtime.tick_ms);
    collision_step(game);
}

game_phase game_get_phase(const game_state *game)
{
    if (game->runtime.awaiting_start)
    {
        return GAME_PHASE_START;
    }

    if (game->dino.status == DINO_STATUS_DEAD)
    {
        return GAME_PHASE_GAME_OVER;
    }

    return GAME_PHASE_PLAYING;
}

bool game_is_playing(const game_state *game)
{
    return game_get_phase(game) == GAME_PHASE_PLAYING;
}

bool game_should_draw_score(const game_state *game)
{
    if (!game->score.achievement_active)
    {
        return true;
    }

    return game->score.flash_ticks >= gameplay_config.score_flash_half_cycle_ticks;
}












