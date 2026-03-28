#ifndef RENDER_H
#define RENDER_H

#include <SDL3/SDL.h>

#include "game.h"

#define DINO_PATH "Resources/dino.png"
#define WINDOW_WIDTH GAME_WORLD_WIDTH
#define WINDOW_HEIGHT GAME_WORLD_HEIGHT

typedef struct render_state render_state;

typedef enum render_sync_mode
{
    RENDER_SYNC_RESET = 0,
    RENDER_SYNC_BEFORE_STEP = 1,
    RENDER_SYNC_AFTER_STEP = 2,
} render_sync_mode;

typedef struct render_context
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *sprite_texture;
    SDL_FRect offset;
    SDL_FRect cut_rect;
    render_state *state;
} render_context;

bool render_init(render_context *ctx);
void render_shutdown(render_context *ctx);
void render_sync_state(render_context *ctx, const game_state *game, render_sync_mode mode);
void render_frame(render_context *ctx, const game_state *game, float alpha, bool debug_collision_boxes, bool debug_background);

#endif
