#include "game.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static void step_game(game_state *game, int step_count)
{
    for (int i = 0; i < step_count; i++)
    {
        game_update_step(game);
    }
}

static void test_start_phase(void)
{
    game_state game;

    srand(0);
    game_state_init(&game);

    assert(game_get_phase(&game) == GAME_PHASE_START);
    assert(!game_is_playing(&game));
    assert(fabsf(game.world.road_scroll_offset) < 0.001f);
}

static void test_jump_starts_game(void)
{
    game_state game;

    srand(0);
    game_state_init(&game);
    game_handle_jump_press(&game);

    assert(game_get_phase(&game) == GAME_PHASE_PLAYING);
    assert(game_is_playing(&game));
    assert(game.dino.status == DINO_STATUS_JUMPING);
}

static void test_score_and_scroll_progress(void)
{
    game_state game;

    srand(0);
    game_state_init(&game);
    game_handle_jump_press(&game);
    step_game(&game, 120);

    assert(game.score.current > 0);
    assert(game.runtime.scroll_speed >= 15);
    assert(game.world.road_scroll_offset > 0.0f);
}

static void test_high_score_dirty_flag(void)
{
    game_state game;
    uint32_t high_score = 0;

    srand(0);
    game_state_init(&game);
    game_handle_jump_press(&game);
    step_game(&game, 8);

    assert(game.score.high > 0);
    assert(game_consume_high_score_dirty(&game, &high_score));
    assert(high_score == game.score.high);
    assert(!game_consume_high_score_dirty(&game, &high_score));
}

static void test_dino_collision_boxes(void)
{
    collision_box boxes[MAX_DINO_COLLISION_BOXES];
    size_t running_count = game_collect_dino_collision_boxes(200.0f, DINO_STATUS_RUNNING, boxes, MAX_DINO_COLLISION_BOXES);
    size_t ducking_count = game_collect_dino_collision_boxes(200.0f, DINO_STATUS_SITTING, boxes, MAX_DINO_COLLISION_BOXES);

    assert(running_count == MAX_DINO_COLLISION_BOXES);
    assert(ducking_count == 1);
}

static void test_obstacle_definitions_are_valid(void)
{
    for (size_t i = 0; i < obstacle_definition_count(); i++)
    {
        const obstacle_definition *definition = obstacle_get_definition((obstacle_type)i);

        assert(definition != NULL);
        assert(definition->sprite_width > 0);
        assert(definition->sprite_height > 0);
        assert(definition->y_position_count > 0);
        assert(definition->collision_box_count > 0);
        if (definition->num_frames > 1)
        {
            assert(definition->frame_ms > 0);
        }
    }
}

static void test_obstacle_spawns_while_running(void)
{
    game_state game;

    srand(0);
    game_state_init(&game);
    game_handle_jump_press(&game);
    step_game(&game, 70);

    assert(game.obstacles.count > 0);
    assert(game.obstacles.count <= MAX_OBSTACLES);
    assert(game.obstacles.items[0].id > 0);
}

static void test_collision_kills_dino(void)
{
    game_state game;
    const obstacle_definition *definition = obstacle_get_definition(OBSTACLE_TYPE_SMALL_CACTUS_1);

    srand(0);
    game_state_init(&game);
    game.runtime.awaiting_start = false;
    game.dino.status = DINO_STATUS_RUNNING;
    game.obstacles.count = 1;
    game.obstacles.items[0].id = 1;
    game.obstacles.items[0].type = OBSTACLE_TYPE_SMALL_CACTUS_1;
    game.obstacles.items[0].x = (float)DINO_WORLD_X + 8.0f;
    game.obstacles.items[0].y = definition->y_positions[0];
    game.obstacles.items[0].speed_offset = 0.0f;
    game.obstacles.items[0].animation_frame = 0;
    game.obstacles.items[0].animation_elapsed_ms = 0.0;

    game_update_step(&game);

    assert(game.dino.status == DINO_STATUS_DEAD);
}

int main(void)
{
    test_start_phase();
    test_jump_starts_game();
    test_score_and_scroll_progress();
    test_high_score_dirty_flag();
    test_dino_collision_boxes();
    test_obstacle_definitions_are_valid();
    test_obstacle_spawns_while_running();
    test_collision_kills_dino();

    puts("all game tests passed");
    return 0;
}
