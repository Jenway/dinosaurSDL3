#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#include "render.h"

#include <stdlib.h>

typedef struct app_state
{
    game_state game;
    render_context render;
    bool previous_jump_down;
    bool debug_collision_boxes;
} app_state;

static bool main_loop_is_jump_down(const bool *keyboard_state)
{
    return keyboard_state[SDL_SCANCODE_SPACE] || keyboard_state[SDL_SCANCODE_W] || keyboard_state[SDL_SCANCODE_UP];
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

static void main_loop_handle_jump_input(app_state *app, bool jump_down)
{
    if (jump_down && !app->previous_jump_down)
    {
        game_handle_jump_press(&app->game);
    }
    else if (!jump_down && app->previous_jump_down)
    {
        game_handle_jump_release(&app->game);
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

    game_state_init(&app->game);
    if (!render_init(&app->render))
    {
        render_shutdown(&app->render);
        free(app);
        return SDL_APP_FAILURE;
    }

    srand((unsigned int)SDL_GetTicks());
    render_sync_state(&app->render, &app->game, RENDER_SYNC_RESET);
    render_frame(&app->render, &app->game, 1.0f, false, false);
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

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    app_state *app = appstate;
    const bool *keyboard_state = SDL_GetKeyboardState(NULL);
    bool jump_down = main_loop_is_jump_down(keyboard_state);
    SDL_AppResult escape_result;

    main_loop_update_duck_input(app, keyboard_state);

    escape_result = main_loop_handle_escape(keyboard_state);
    if (escape_result != SDL_APP_CONTINUE)
    {
        return escape_result;
    }

    main_loop_handle_jump_input(app, jump_down);
    if (game_is_playing(&app->game))
    {
        game_update_step(&app->game);
    }
    else
    {
        render_sync_state(&app->render, &app->game, RENDER_SYNC_RESET);
    }

    render_sync_state(&app->render, &app->game, RENDER_SYNC_RESET);
    render_frame(&app->render, &app->game, 1.0f, app->debug_collision_boxes, false);
    app->previous_jump_down = jump_down;
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    app_state *app = appstate;

    (void)result;

    if (app != NULL)
    {
        render_shutdown(&app->render);
        free(app);
    }
}
