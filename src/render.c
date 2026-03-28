#include "render.h"

#include "stb_image.h"

typedef struct render_snapshot
{
    float dino_jump_height;
    float road_scroll_offset;
    float cloud_x[MAX_CLOUDS];
    uint32_t cloud_id[MAX_CLOUDS];
    size_t cloud_count;
    float night_blend;
    float moon_x;
    float star_x[NIGHT_STAR_COUNT];
    float obstacle_x[MAX_OBSTACLES];
    uint32_t obstacle_id[MAX_OBSTACLES];
    size_t obstacle_count;
} render_snapshot;

struct render_state
{
    render_snapshot previous;
    render_snapshot current;
};

typedef struct sprite_rect
{
    int source_x;
    int source_y;
    int width;
    int height;
} sprite_rect;

static const int dino_screen_x = DINO_WORLD_X - 2;
static const int dino_ground_y = DINO_WORLD_Y;
static const int road_screen_y = WINDOW_HEIGHT - 25;
static const int road_baseline_y = WINDOW_HEIGHT - 13;
static const int moon_screen_y = 30;
static const int score_screen_y = 5;
static const sprite_rect dino_dead_sprite = {2118, 2, 88, 94};
static const sprite_rect cloud_sprite = {166, 2, 92, 28};
static const sprite_rect road_sprite = {2, 104, ROAD_TILE_WIDTH, 24};
static const sprite_rect restart_sprite = {2, 130, 72, 64};
static const sprite_rect game_over_text_sprite = {1294, 28, 382, 22};

static const sprite_rect dino_run_sprites[] = {
    {1854, 2, 88, 94},
    {1942, 2, 88, 94},
};

static const sprite_rect dino_idle_sprite = {1678, 2, 88, 94};

static const sprite_rect dino_sit_sprites[] = {
    {2206, 2, 118, 94},
    {2324, 2, 118, 94},
};

static const int moon_phase_source_offsets[] = {280, 240, 200, 120, 80, 40, 0};
static const int moon_phase_output_widths[] = {20, 20, 20, 40, 20, 20, 20};
static const float moon_wrap_span = 1040.0f;
static const float star_wrap_span = 1080.0f;

static SDL_Texture *render_load_texture(SDL_Renderer *renderer, const char *path)
{
    SDL_Surface *surface;
    SDL_Texture *texture;
    Uint8 *file_bytes;
    stbi_uc *pixels;
    size_t file_size = 0;
    int width;
    int height;
    int channels;

    file_bytes = SDL_LoadFile(path, &file_size);
    if (file_bytes == NULL)
    {
        SDL_Log("Could not read texture file: %s", SDL_GetError());
        return NULL;
    }

    pixels = stbi_load_from_memory(file_bytes, (int)file_size, &width, &height, &channels, STBI_rgb_alpha);
    SDL_free(file_bytes);
    if (pixels == NULL)
    {
        SDL_Log("Could not decode texture file: %s", stbi_failure_reason());
        return NULL;
    }

    surface = SDL_CreateSurfaceFrom(width, height, SDL_PIXELFORMAT_RGBA32, pixels, width * STBI_rgb_alpha);
    if (surface == NULL)
    {
        SDL_Log("Could not create surface from pixels: %s", SDL_GetError());
        stbi_image_free(pixels);
        return NULL;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    stbi_image_free(pixels);

    if (texture == NULL)
    {
        SDL_Log("Could not create texture: %s", SDL_GetError());
        return NULL;
    }

    return texture;
}

static float render_lerp(float previous, float current, float alpha)
{
    return previous + (current - previous) * alpha;
}

static float render_lerp_wrapped(float previous, float current, float alpha, float wrap_span)
{
    float half_span = wrap_span * 0.5f;

    if (current - previous > half_span)
    {
        current -= wrap_span;
    }
    else if (previous - current > half_span)
    {
        previous -= wrap_span;
    }

    return render_lerp(previous, current, alpha);
}

static float render_find_snapshot_value(const float *values, size_t count, size_t index, float fallback)
{
    if (index >= count)
    {
        return fallback;
    }

    return values[index];
}

static void render_capture_snapshot(render_snapshot *snapshot, const game_state *game)
{
    snapshot->dino_jump_height = (float)game->dino.jump_height;
    snapshot->road_scroll_offset = game->world.road_scroll_offset;
    snapshot->cloud_count = game->world.cloud_count;
    snapshot->night_blend = game->world.night_blend;
    snapshot->moon_x = game->world.moon_x;
    snapshot->obstacle_count = game->obstacles.count;

    for (size_t i = 0; i < game->world.cloud_count; i++)
    {
        snapshot->cloud_x[i] = game->world.clouds[i].x;
        snapshot->cloud_id[i] = game->world.clouds[i].id;
    }

    for (int i = 0; i < NIGHT_STAR_COUNT; i++)
    {
        snapshot->star_x[i] = game->world.stars[i].x;
    }

    for (size_t i = 0; i < game->obstacles.count; i++)
    {
        snapshot->obstacle_x[i] = game->obstacles.items[i].x;
        snapshot->obstacle_id[i] = game->obstacles.items[i].id;
    }
}

static bool render_find_snapshot_cloud_x(const render_snapshot *snapshot, uint32_t cloud_id, float *x)
{
    for (size_t i = 0; i < snapshot->cloud_count; i++)
    {
        if (snapshot->cloud_id[i] == cloud_id)
        {
            *x = snapshot->cloud_x[i];
            return true;
        }
    }

    return false;
}

static bool render_find_snapshot_obstacle_x(const render_snapshot *snapshot, uint32_t obstacle_id, float *x)
{
    for (size_t i = 0; i < snapshot->obstacle_count; i++)
    {
        if (snapshot->obstacle_id[i] == obstacle_id)
        {
            *x = snapshot->obstacle_x[i];
            return true;
        }
    }

    return false;
}

static void render_set_draw_color(render_context *ctx, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    SDL_SetRenderDrawColor(ctx->renderer, r, g, b, a);
}

static void render_draw_source(render_context *ctx,
                               float x,
                               float y,
                               int source_x,
                               int source_y,
                               int source_width,
                               int source_height,
                               float destination_width,
                               float destination_height)
{
    ctx->offset.x = SDL_roundf(x);
    ctx->offset.y = SDL_roundf(y);
    ctx->offset.w = SDL_roundf(destination_width);
    ctx->offset.h = SDL_roundf(destination_height);

    ctx->cut_rect.x = (float)source_x;
    ctx->cut_rect.y = (float)source_y;
    ctx->cut_rect.w = (float)source_width;
    ctx->cut_rect.h = (float)source_height;

    SDL_RenderTexture(ctx->renderer, ctx->sprite_texture, &ctx->cut_rect, &ctx->offset);
}

static void render_draw_sprite(render_context *ctx, float x, float y, sprite_rect sprite)
{
    render_draw_source(ctx, x, y, sprite.source_x, sprite.source_y, sprite.width, sprite.height, (float)sprite.width, (float)sprite.height);
}

static void render_draw_scaled_sprite(render_context *ctx, float x, float y, sprite_rect sprite, float output_width, float output_height)
{
    render_draw_source(ctx, x, y, sprite.source_x, sprite.source_y, sprite.width, sprite.height, output_width, output_height);
}

static void render_draw_rect_outline(render_context *ctx, float x, float y, float width, float height)
{
    SDL_FRect rect = {
        .x = SDL_roundf(x),
        .y = SDL_roundf(y),
        .w = SDL_roundf(width),
        .h = SDL_roundf(height),
    };

    SDL_RenderRect(ctx->renderer, &rect);
}

static void render_draw_digit(render_context *ctx, char digit, float x, float y)
{
    int source_x = 1294;
    int source_y = 2;

    if (digit >= '0' && digit <= '9')
    {
        source_x += (digit - '0') * 20;
    }
    else if (digit == 'H')
    {
        source_x += 200;
    }
    else if (digit == 'I')
    {
        source_x += 220;
    }
    else
    {
        return;
    }

    render_draw_source(ctx, x, y, source_x, source_y, 20, 26, 10.0f, 13.0f);
}

static void render_draw_score_text(render_context *ctx, const char *text, float x, float y)
{
    float cursor_x = x;

    for (const char *it = text; *it != '\0'; it++)
    {
        if (*it == ' ')
        {
            cursor_x += 6.0f;
            continue;
        }

        render_draw_digit(ctx, *it, cursor_x, y);
        cursor_x += 11.0f;
    }
}

static float render_interpolate_cloud_x(const game_state *game,
                                        const render_snapshot *previous,
                                        const render_snapshot *current,
                                        size_t index,
                                        float alpha)
{
    const cloud_state *cloud = &game->world.clouds[index];
    float previous_x;
    float current_x;

    if (render_find_snapshot_cloud_x(previous, cloud->id, &previous_x) &&
        render_find_snapshot_cloud_x(current, cloud->id, &current_x))
    {
        return render_lerp(previous_x, current_x, alpha);
    }

    return cloud->x;
}

static float render_interpolate_obstacle_x(const game_state *game,
                                           const render_snapshot *previous,
                                           const render_snapshot *current,
                                           size_t index,
                                           float alpha)
{
    const obstacle_state *obstacle = &game->obstacles.items[index];
    float previous_x;
    float current_x;

    if (render_find_snapshot_obstacle_x(previous, obstacle->id, &previous_x) &&
        render_find_snapshot_obstacle_x(current, obstacle->id, &current_x))
    {
        return render_lerp(previous_x, current_x, alpha);
    }

    return obstacle->x;
}

static float render_interpolate_moon_x(const render_snapshot *previous,
                                       const render_snapshot *current,
                                       float alpha)
{
    return render_lerp_wrapped(previous->moon_x, current->moon_x, alpha, moon_wrap_span);
}

static float render_interpolate_star_x(const game_state *game,
                                       const render_snapshot *previous,
                                       const render_snapshot *current,
                                       int index,
                                       float alpha)
{
    float previous_x = render_find_snapshot_value(previous->star_x, NIGHT_STAR_COUNT, (size_t)index, game->world.stars[index].x);
    float current_x = render_find_snapshot_value(current->star_x, NIGHT_STAR_COUNT, (size_t)index, game->world.stars[index].x);

    return render_lerp_wrapped(previous_x, current_x, alpha, star_wrap_span);
}

static void render_draw_night_sky(render_context *ctx,
                                  const game_state *game,
                                  const render_snapshot *previous,
                                  const render_snapshot *current,
                                  float alpha)
{
    float night_blend = render_lerp(previous->night_blend, current->night_blend, alpha);
    int moon_phase = game->world.moon_phase % (int)(sizeof(moon_phase_source_offsets) / sizeof(moon_phase_source_offsets[0]));
    int moon_output_width = moon_phase_output_widths[moon_phase];
    int moon_source_width = moon_output_width * 2;

    if (night_blend <= 0.0f)
    {
        return;
    }

    SDL_SetTextureAlphaModFloat(ctx->sprite_texture, night_blend);

    for (int i = 0; i < NIGHT_STAR_COUNT; i++)
    {
        int source_y = 2 + game->world.stars[i].variant * 18;
        float star_x = render_interpolate_star_x(game, previous, current, i, alpha);

        render_draw_source(ctx, star_x, (float)game->world.stars[i].y, 1276, source_y, 18, 18, 9.0f, 9.0f);
    }

    render_draw_source(ctx,
                       render_interpolate_moon_x(previous, current, alpha),
                       (float)moon_screen_y,
                       954 + moon_phase_source_offsets[moon_phase],
                       2,
                       moon_source_width,
                       80,
                       (float)moon_output_width,
                       40.0f);

    SDL_SetTextureAlphaModFloat(ctx->sprite_texture, 1.0f);
}

static void render_draw_background_debug(render_context *ctx,
                                         const game_state *game,
                                         const render_snapshot *previous,
                                         const render_snapshot *current,
                                         float alpha)
{
    int moon_phase = game->world.moon_phase % (int)(sizeof(moon_phase_source_offsets) / sizeof(moon_phase_source_offsets[0]));
    float moon_width = (float)moon_phase_output_widths[moon_phase];

    render_set_draw_color(ctx, 255, 96, 96, 255);
    SDL_RenderLine(ctx->renderer, 0.0f, (float)road_baseline_y, (float)WINDOW_WIDTH, (float)road_baseline_y);

    render_set_draw_color(ctx, 64, 200, 255, 255);
    for (size_t i = 0; i < game->world.cloud_count; i++)
    {
        render_draw_rect_outline(ctx,
                                 render_interpolate_cloud_x(game, previous, current, i, alpha),
                                 (float)game->world.clouds[i].y,
                                 (float)cloud_sprite.width,
                                 (float)cloud_sprite.height);
    }

    if (render_lerp(previous->night_blend, current->night_blend, alpha) <= 0.0f)
    {
        return;
    }

    render_set_draw_color(ctx, 255, 220, 80, 255);
    render_draw_rect_outline(ctx,
                             render_interpolate_moon_x(previous, current, alpha),
                             (float)moon_screen_y,
                             moon_width,
                             40.0f);

    render_set_draw_color(ctx, 180, 180, 255, 255);
    for (int i = 0; i < NIGHT_STAR_COUNT; i++)
    {
        render_draw_rect_outline(ctx,
                                 render_interpolate_star_x(game, previous, current, i, alpha),
                                 (float)game->world.stars[i].y,
                                 9.0f,
                                 9.0f);
    }
}

static void render_draw_hud(render_context *ctx,
                            const game_state *game,
                            bool debug_collision_boxes,
                            bool debug_background)
{
    char current_score[8];
    char high_score[16];
    float night_blend = game->world.night_blend;
    Uint8 text_value = (Uint8)SDL_roundf(render_lerp(32.0f, 240.0f, night_blend));
    float debug_y = 16.0f;

    render_set_draw_color(ctx, text_value, text_value, text_value, 255);

    if (game_get_phase(game) == GAME_PHASE_START)
    {
        SDL_RenderDebugText(ctx->renderer, 16.0f, 16.0f, "SPACE/W/UP to start");
        SDL_RenderDebugText(ctx->renderer, 16.0f, 32.0f, "Hold S/DOWN to duck");
        SDL_RenderDebugText(ctx->renderer, 16.0f, 48.0f, debug_collision_boxes ? "F1 collision boxes: ON" : "F1 collision boxes: OFF");
        SDL_RenderDebugText(ctx->renderer, 16.0f, 64.0f, debug_background ? "F2 background debug: ON" : "F2 background debug: OFF");
        return;
    }

    SDL_snprintf(current_score, sizeof(current_score), "%05u", game->score.current);
    SDL_snprintf(high_score, sizeof(high_score), "HI %05u", game->score.high);

    render_draw_score_text(ctx, high_score, WINDOW_WIDTH - 150.0f, (float)score_screen_y);
    if (game_should_draw_score(game))
    {
        render_draw_score_text(ctx, current_score, WINDOW_WIDTH - 62.0f, (float)score_screen_y);
    }

    if (debug_collision_boxes)
    {
        SDL_RenderDebugText(ctx->renderer, 16.0f, debug_y, "F1 collision boxes: ON");
        debug_y += 16.0f;
    }

    if (debug_background)
    {
        SDL_RenderDebugText(ctx->renderer, 16.0f, debug_y, "F2 background debug: ON");
    }

    if (game_get_phase(game) == GAME_PHASE_GAME_OVER)
    {
        render_draw_scaled_sprite(ctx,
                                  (WINDOW_WIDTH - 191.0f) * 0.5f,
                                  78.0f,
                                  game_over_text_sprite,
                                  191.0f,
                                  11.0f);
        render_draw_scaled_sprite(ctx,
                                  (WINDOW_WIDTH - 36.0f) * 0.5f,
                                  112.0f,
                                  restart_sprite,
                                  36.0f,
                                  32.0f);
        SDL_RenderDebugText(ctx->renderer, 322.0f, 154.0f, "SPACE/W/UP to restart");
    }
}

static void render_draw_dino(render_context *ctx,
                             const game_state *game,
                             const render_snapshot *previous,
                             const render_snapshot *current,
                             float alpha)
{
    const dino_state *dino = &game->dino;
    const sprite_rect *dino_sprite = &dino_idle_sprite;
    float interpolated_jump_height = render_lerp(previous->dino_jump_height, current->dino_jump_height, alpha);
    float dino_y = (float)dino_ground_y - interpolated_jump_height;
    int frame_index;

    if (dino->status == DINO_STATUS_RUNNING)
    {
        frame_index = dino->running_frame % (int)(sizeof(dino_run_sprites) / sizeof(dino_run_sprites[0]));
        dino_sprite = &dino_run_sprites[frame_index];
    }
    else if (dino->status == DINO_STATUS_SITTING)
    {
        frame_index = dino->running_frame % (int)(sizeof(dino_sit_sprites) / sizeof(dino_sit_sprites[0]));
        dino_sprite = &dino_sit_sprites[frame_index];
    }
    else if (dino->status == DINO_STATUS_DEAD)
    {
        dino_sprite = &dino_dead_sprite;
    }

    render_draw_sprite(ctx, (float)dino_screen_x, dino_y, *dino_sprite);
}

static void render_draw_road(render_context *ctx,
                             const render_snapshot *previous,
                             const render_snapshot *current,
                             float alpha)
{
    SDL_FRect source = {
        .x = (float)road_sprite.source_x,
        .y = (float)road_sprite.source_y,
        .w = (float)road_sprite.width,
        .h = (float)road_sprite.height,
    };
    float interpolated_road_x = render_lerp(previous->road_scroll_offset, current->road_scroll_offset, alpha);
    SDL_FRect destination = {
        .x = SDL_roundf(-SDL_fmodf(interpolated_road_x, (float)road_sprite.width)),
        .y = (float)road_screen_y,
        .w = (float)(WINDOW_WIDTH + road_sprite.width),
        .h = (float)road_sprite.height,
    };

    SDL_RenderTextureTiled(ctx->renderer, ctx->sprite_texture, &source, 1.0f, &destination);
}

static void render_draw_clouds(render_context *ctx,
                               const game_state *game,
                               const render_snapshot *previous,
                               const render_snapshot *current,
                               float alpha)
{
    for (size_t i = 0; i < game->world.cloud_count; i++)
    {
        render_draw_sprite(ctx,
                           render_interpolate_cloud_x(game, previous, current, i, alpha),
                           (float)game->world.clouds[i].y,
                           cloud_sprite);
    }
}

static void render_draw_obstacles(render_context *ctx,
                                  const game_state *game,
                                  const render_snapshot *previous,
                                  const render_snapshot *current,
                                  float alpha)
{
    for (size_t i = 0; i < game->obstacles.count; i++)
    {
        const obstacle_state *obstacle = &game->obstacles.items[i];
        const obstacle_definition *definition = obstacle_get_definition(obstacle->type);
        int source_x = definition->sprite_x + obstacle->animation_frame * definition->sprite_width;

        render_draw_sprite(ctx,
                           render_interpolate_obstacle_x(game, previous, current, i, alpha),
                           (float)obstacle->y,
                           (sprite_rect){source_x, definition->sprite_y, definition->sprite_width, definition->sprite_height});
    }
}

static void render_draw_collision_boxes(render_context *ctx,
                                        const game_state *game,
                                        const render_snapshot *previous,
                                        const render_snapshot *current,
                                        float alpha)
{
    collision_box dino_boxes[MAX_DINO_COLLISION_BOXES];
    float interpolated_jump_height = render_lerp(previous->dino_jump_height, current->dino_jump_height, alpha);
    size_t dino_box_count;

    render_set_draw_color(ctx, 255, 48, 48, 255);

    dino_box_count = game_collect_dino_collision_boxes(interpolated_jump_height, game->dino.status, dino_boxes, MAX_DINO_COLLISION_BOXES);
    for (size_t i = 0; i < dino_box_count; i++)
    {
        render_draw_rect_outline(ctx,
                                 (float)dino_boxes[i].x,
                                 (float)dino_boxes[i].y,
                                 (float)dino_boxes[i].width,
                                 (float)dino_boxes[i].height);
    }

    for (size_t i = 0; i < game->obstacles.count; i++)
    {
        const obstacle_state *obstacle = &game->obstacles.items[i];
        const obstacle_definition *definition = obstacle_get_definition(obstacle->type);
        float obstacle_x = render_interpolate_obstacle_x(game, previous, current, i, alpha);

        for (size_t box_index = 0; box_index < definition->collision_box_count; box_index++)
        {
            const collision_box *box = &definition->collision_boxes[box_index];

            render_draw_rect_outline(ctx,
                                     obstacle_x + (float)(box->x * GAME_SPRITE_SCALE),
                                     (float)obstacle->y + (float)(box->y * GAME_SPRITE_SCALE),
                                     (float)(box->width * GAME_SPRITE_SCALE),
                                     (float)(box->height * GAME_SPRITE_SCALE));
        }
    }
}

bool render_init(render_context *ctx)
{
    ctx->state = SDL_calloc(1, sizeof(*ctx->state));
    if (ctx->state == NULL)
    {
        SDL_Log("Could not allocate render state");
        return false;
    }

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return false;
    }

    ctx->window = SDL_CreateWindow("Hello,dinosaur", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_BORDERLESS);
    if (ctx->window == NULL)
    {
        SDL_Log("Could not create window: %s", SDL_GetError());
        return false;
    }

    ctx->renderer = SDL_CreateRenderer(ctx->window, NULL);
    if (ctx->renderer == NULL)
    {
        SDL_Log("Could not create renderer: %s", SDL_GetError());
        return false;
    }

    if (!SDL_SetRenderVSync(ctx->renderer, 1))
    {
        SDL_Log("Could not enable renderer vsync: %s", SDL_GetError());
    }

    ctx->sprite_texture = render_load_texture(ctx->renderer, DINO_PATH);
    if (ctx->sprite_texture == NULL)
    {
        return false;
    }

    if (!SDL_SetTextureScaleMode(ctx->sprite_texture, SDL_SCALEMODE_NEAREST))
    {
        SDL_Log("Could not set texture scale mode: %s", SDL_GetError());
        return false;
    }

    if (!SDL_SetTextureBlendMode(ctx->sprite_texture, SDL_BLENDMODE_BLEND))
    {
        SDL_Log("Could not set texture blend mode: %s", SDL_GetError());
        return false;
    }

    return true;
}

void render_shutdown(render_context *ctx)
{
    if (ctx->state != NULL)
    {
        SDL_free(ctx->state);
        ctx->state = NULL;
    }
    if (ctx->sprite_texture != NULL)
    {
        SDL_DestroyTexture(ctx->sprite_texture);
    }
    if (ctx->renderer != NULL)
    {
        SDL_DestroyRenderer(ctx->renderer);
    }
    if (ctx->window != NULL)
    {
        SDL_DestroyWindow(ctx->window);
    }
    SDL_Quit();
}

void render_sync_state(render_context *ctx, const game_state *game, render_sync_mode mode)
{
    if (ctx->state == NULL)
    {
        return;
    }

    if (mode == RENDER_SYNC_RESET)
    {
        render_capture_snapshot(&ctx->state->current, game);
        ctx->state->previous = ctx->state->current;
    }
    else if (mode == RENDER_SYNC_BEFORE_STEP)
    {
        render_capture_snapshot(&ctx->state->previous, game);
    }
    else if (mode == RENDER_SYNC_AFTER_STEP)
    {
        render_capture_snapshot(&ctx->state->current, game);
    }
}

void render_frame(render_context *ctx, const game_state *game, float alpha, bool debug_collision_boxes, bool debug_background)
{
    const render_snapshot *previous;
    const render_snapshot *current;
    float night_blend;
    Uint8 background_r;
    Uint8 background_g;
    Uint8 background_b;

    if (ctx->state == NULL)
    {
        return;
    }

    previous = &ctx->state->previous;
    current = &ctx->state->current;
    night_blend = render_lerp(previous->night_blend, current->night_blend, alpha);
    background_r = (Uint8)SDL_roundf(render_lerp(255.0f, 27.0f, night_blend));
    background_g = (Uint8)SDL_roundf(render_lerp(255.0f, 29.0f, night_blend));
    background_b = (Uint8)SDL_roundf(render_lerp(255.0f, 43.0f, night_blend));

    render_set_draw_color(ctx, background_r, background_g, background_b, 255);
    SDL_RenderClear(ctx->renderer);

    render_draw_night_sky(ctx, game, previous, current, alpha);
    render_draw_clouds(ctx, game, previous, current, alpha);
    render_draw_road(ctx, previous, current, alpha);
    render_draw_obstacles(ctx, game, previous, current, alpha);
    render_draw_dino(ctx, game, previous, current, alpha);
    if (debug_background)
    {
        render_draw_background_debug(ctx, game, previous, current, alpha);
    }
    if (debug_collision_boxes)
    {
        render_draw_collision_boxes(ctx, game, previous, current, alpha);
    }
    render_draw_hud(ctx, game, debug_collision_boxes, debug_background);
    SDL_RenderPresent(ctx->renderer);
}
