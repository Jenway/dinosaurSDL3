#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#include "persistence.h"
#include "render.h"

#include <stdlib.h>

#define MAX_CATCHUP_STEPS 5

typedef struct app_state
{
    game_state game;
    persistence_context persistence;
    render_context render;
    bool previous_jump_down;
    bool debug_collision_boxes;
    bool debug_background;
} app_state;

static bool main_loop_is_jump_down(const bool *keyboard_state)
{
    return keyboard_state[SDL_SCANCODE_SPACE] || keyboard_state[SDL_SCANCODE_W] || keyboard_state[SDL_SCANCODE_UP];
}

static double main_loop_now_ms(void)
{
    return (double)SDL_GetTicksNS() / 1000000.0;
}

static float main_loop_get_alpha(const runtime_state *runtime)
{
    double alpha;

    if (runtime->tick_ms <= 0.0)
    {
        return 1.0f;
    }

    alpha = runtime->accumulator_ms / runtime->tick_ms;
    if (alpha < 0.0)
    {
        return 0.0f;
    }
    if (alpha > 1.0)
    {
        return 1.0f;
    }

    return (float)alpha;
}

static void main_loop_reset_snapshots(app_state *app)
{
    render_sync_state(&app->render, &app->game, RENDER_SYNC_RESET);
}

static void main_loop_reset_timing(runtime_state *runtime, double now_ms)
{
    runtime->last_tick_ms = now_ms;
    runtime->accumulator_ms = 0.0;
}

static void main_loop_sync_persistence(app_state *app)
{
    uint32_t high_score;

    persistence_update(&app->persistence);
    if (persistence_consume_loaded_high_score(&app->persistence, &high_score))
    {
        game_set_high_score(&app->game, high_score);
    }

    if (game_consume_high_score_dirty(&app->game, &high_score))
    {
        persistence_request_high_score_save(&app->persistence, high_score);
    }

    persistence_update(&app->persistence);
}

static void main_loop_update_duck_input(app_state *app, const bool *keyboard_state)
{
    bool duck_held = keyboard_state[SDL_SCANCODE_S] || keyboard_state[SDL_SCANCODE_DOWN];

    if (game_is_playing(&app->game))
    {
        game_set_duck_held(&app->game, duck_held);
    }
    else
    {
        game_set_duck_held(&app->game, false);
    }
}

static SDL_AppResult main_loop_handle_escape(const bool *keyboard_state)
{
    if (keyboard_state[SDL_SCANCODE_ESCAPE])
    {
        return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}

static void main_loop_handle_jump_input(app_state *app, bool jump_down, double now_ms)
{
    game_phase previous_phase = game_get_phase(&app->game);
    game_phase current_phase;

    if (jump_down && !app->previous_jump_down)
    {
        game_handle_jump_press(&app->game);
    }
    else if (!jump_down && app->previous_jump_down)
    {
        game_handle_jump_release(&app->game);
    }

    current_phase = game_get_phase(&app->game);
    if (previous_phase != GAME_PHASE_PLAYING && current_phase == GAME_PHASE_PLAYING)
    {
        main_loop_reset_timing(&app->game.runtime, now_ms);
        main_loop_reset_snapshots(app);
    }
}

static void main_loop_run_fixed_steps(app_state *app, double now_ms)
{
    runtime_state *runtime = &app->game.runtime;
    double elapsed_ms;
    double max_elapsed_ms = runtime->tick_ms * MAX_CATCHUP_STEPS;
    int step_count = 0;
    bool snapshot_captured = false;

    if (runtime->last_tick_ms <= 0.0)
    {
        main_loop_reset_timing(runtime, now_ms);
        return;
    }

    elapsed_ms = now_ms - runtime->last_tick_ms;
    if (elapsed_ms > max_elapsed_ms)
    {
        elapsed_ms = max_elapsed_ms;
    }

    runtime->last_tick_ms = now_ms;
    runtime->accumulator_ms += elapsed_ms;

    while (runtime->accumulator_ms >= runtime->tick_ms && step_count < MAX_CATCHUP_STEPS)
    {
        if (!snapshot_captured)
        {
            render_sync_state(&app->render, &app->game, RENDER_SYNC_BEFORE_STEP);
            snapshot_captured = true;
        }

        runtime->accumulator_ms -= runtime->tick_ms;
        game_update_step(&app->game);
        step_count += 1;

        if (app->game.dino.status == DINO_STATUS_DEAD)
        {
            game_set_duck_held(&app->game, false);
            main_loop_reset_timing(runtime, now_ms);
            main_loop_reset_snapshots(app);
            return;
        }
    }

    if (snapshot_captured)
    {
        render_sync_state(&app->render, &app->game, RENDER_SYNC_AFTER_STEP);
    }

    if (runtime->accumulator_ms >= runtime->tick_ms)
    {
        runtime->accumulator_ms = 0.0;
    }
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    app_state *app = calloc(1, sizeof(*app));

    (void)argc;
    (void)argv;

    if (app == NULL)
    {
        SDL_Log("Could not allocate app state");
        return SDL_APP_FAILURE;
    }

    SDL_SetHint(SDL_HINT_MAIN_CALLBACK_RATE, "60");
    game_state_init(&app->game);
    persistence_init(&app->persistence);

    if (!render_init(&app->render))
    {
        render_shutdown(&app->render);
        free(app);
        return SDL_APP_FAILURE;
    }

    srand((unsigned int)SDL_GetTicks());
    main_loop_sync_persistence(app);
    main_loop_reset_timing(&app->game.runtime, main_loop_now_ms());
    main_loop_reset_snapshots(app);
    render_frame(&app->render,
                 &app->game,
                 1.0f,
                 app->debug_collision_boxes,
                 app->debug_background);
    *appstate = app;
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    app_state *app = appstate;

    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }

    if (event->type == SDL_EVENT_KEY_DOWN && !event->key.repeat && event->key.scancode == SDL_SCANCODE_F1)
    {
        app->debug_collision_boxes = !app->debug_collision_boxes;
    }
    else if (event->type == SDL_EVENT_KEY_DOWN && !event->key.repeat && event->key.scancode == SDL_SCANCODE_F2)
    {
        app->debug_background = !app->debug_background;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    app_state *app = appstate;
    const bool *keyboard_state = SDL_GetKeyboardState(NULL);
    bool jump_down = main_loop_is_jump_down(keyboard_state);
    double now_ms = main_loop_now_ms();
    SDL_AppResult escape_result;
    float alpha = 1.0f;

    main_loop_sync_persistence(app);
    main_loop_update_duck_input(app, keyboard_state);

    escape_result = main_loop_handle_escape(keyboard_state);
    if (escape_result != SDL_APP_CONTINUE)
    {
        return escape_result;
    }

    main_loop_handle_jump_input(app, jump_down, now_ms);

    if (game_is_playing(&app->game))
    {
        main_loop_run_fixed_steps(app, now_ms);
        alpha = main_loop_get_alpha(&app->game.runtime);
    }
    else
    {
        main_loop_reset_timing(&app->game.runtime, now_ms);
        main_loop_reset_snapshots(app);
    }

    main_loop_sync_persistence(app);
    render_frame(&app->render,
                 &app->game,
                 alpha,
                 app->debug_collision_boxes,
                 app->debug_background);
    app->previous_jump_down = jump_down;
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    app_state *app = appstate;
    uint32_t high_score;

    (void)result;

    if (app != NULL)
    {
        if (game_consume_high_score_dirty(&app->game, &high_score))
        {
            persistence_request_high_score_save(&app->persistence, high_score);
        }
        persistence_shutdown(&app->persistence);
        render_shutdown(&app->render);
        free(app);
    }
}
