#include "persistence.h"

#include <SDL3/SDL.h>

#include <stdlib.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>

/* Web builds use localStorage directly for the single high-score value. This is
 * simpler and more reliable here than the current SDL storage backend behavior. */
EM_JS(int, persistence_web_load_high_score, (), {
    try {
        if (!globalThis.localStorage) {
            return -1;
        }

        const rawValue = globalThis.localStorage.getItem('dino-c.high_score');
        if (rawValue === null) {
            return -1;
        }

        const parsedValue = Number.parseInt(rawValue, 10);
        if (!Number.isFinite(parsedValue) || parsedValue < 0) {
            return -1;
        }

        return parsedValue >>> 0;
    } catch (error) {
        return -2;
    }
});

EM_JS(int, persistence_web_save_high_score, (unsigned int highScore), {
    try {
        if (!globalThis.localStorage) {
            return 0;
        }

        globalThis.localStorage.setItem('dino-c.high_score', String(highScore >>> 0));
        return 1;
    } catch (error) {
        return 0;
    }
});
#else
static const char *const persistence_org = "DinoC";
static const char *const persistence_app = "Dino C";
static const char *const persistence_high_score_path = "high_score.txt";

static void persistence_close_storage(persistence_context *context)
{
    if (context->storage != NULL)
    {
        SDL_CloseStorage(context->storage);
        context->storage = NULL;
    }
}

static bool persistence_open_storage(persistence_context *context)
{
    if (context->storage != NULL)
    {
        return true;
    }

    context->storage = SDL_OpenUserStorage(persistence_org, persistence_app, 0);
    if (context->storage == NULL)
    {
        if (!context->storage_error_logged)
        {
            SDL_Log("Could not open user storage: %s", SDL_GetError());
            context->storage_error_logged = true;
        }

        return false;
    }

    context->storage_error_logged = false;
    return true;
}

static void persistence_finish_load(persistence_context *context)
{
    Uint64 file_size = 0;

    if (SDL_GetStorageFileSize(context->storage, persistence_high_score_path, &file_size) && file_size > 0)
    {
        char *buffer = SDL_malloc((size_t)file_size + 1);

        if (buffer != NULL && SDL_ReadStorageFile(context->storage, persistence_high_score_path, buffer, file_size))
        {
            char *end = NULL;
            unsigned long parsed_high_score;

            buffer[file_size] = '\0';
            parsed_high_score = strtoul(buffer, &end, 10);
            if (end != buffer && parsed_high_score <= UINT32_MAX)
            {
                context->loaded_high_score = (uint32_t)parsed_high_score;
                context->loaded_value_available = true;
            }
        }

        SDL_free(buffer);
    }

    context->load_completed = true;
    context->operation = PERSISTENCE_OPERATION_NONE;
    persistence_close_storage(context);
}

static void persistence_finish_save(persistence_context *context)
{
    char buffer[16];
    int buffer_length = SDL_snprintf(buffer, sizeof(buffer), "%u", context->pending_high_score);

    if (buffer_length > 0)
    {
        if (!SDL_WriteStorageFile(context->storage, persistence_high_score_path, buffer, (Uint64)buffer_length))
        {
            SDL_Log("Could not write high score: %s", SDL_GetError());
            return;
        }
    }

    context->save_pending = false;
    context->operation = PERSISTENCE_OPERATION_NONE;
    persistence_close_storage(context);
}
#endif

void persistence_init(persistence_context *context)
{
    SDL_zero(*context);
#ifdef __EMSCRIPTEN__
    context->load_completed = true;

    {
        int loaded_high_score = persistence_web_load_high_score();

        if (loaded_high_score >= 0)
        {
            context->loaded_high_score = (uint32_t)loaded_high_score;
            context->loaded_value_available = true;
        }
        else if (loaded_high_score == -2)
        {
            context->storage_error_logged = true;
            SDL_Log("Could not access browser localStorage for high score");
        }
    }
#else
    context->operation = PERSISTENCE_OPERATION_LOAD;
#endif
}

void persistence_update(persistence_context *context)
{
#ifdef __EMSCRIPTEN__
    if (!context->save_pending)
    {
        return;
    }

    if (!persistence_web_save_high_score(context->pending_high_score))
    {
        if (!context->storage_error_logged)
        {
            SDL_Log("Could not save high score to browser localStorage");
            context->storage_error_logged = true;
        }
        return;
    }

    context->storage_error_logged = false;
    context->save_pending = false;
    return;
#else
    if (context->operation == PERSISTENCE_OPERATION_NONE)
    {
        if (!context->load_completed)
        {
            context->operation = PERSISTENCE_OPERATION_LOAD;
        }
        else if (context->save_pending)
        {
            context->operation = PERSISTENCE_OPERATION_SAVE;
        }
        else
        {
            return;
        }
    }

    if (!persistence_open_storage(context))
    {
        return;
    }

    if (!SDL_StorageReady(context->storage))
    {
        return;
    }

    if (context->operation == PERSISTENCE_OPERATION_LOAD)
    {
        persistence_finish_load(context);
    }
    else if (context->operation == PERSISTENCE_OPERATION_SAVE)
    {
        persistence_finish_save(context);
    }
#endif
}

bool persistence_consume_loaded_high_score(persistence_context *context, uint32_t *high_score)
{
    if (!context->loaded_value_available)
    {
        return false;
    }

    context->loaded_value_available = false;
    *high_score = context->loaded_high_score;
    return true;
}

void persistence_request_high_score_save(persistence_context *context, uint32_t high_score)
{
    if (!context->save_pending || high_score > context->pending_high_score)
    {
        context->pending_high_score = high_score;
    }

    context->save_pending = true;
}

void persistence_shutdown(persistence_context *context)
{
#ifdef __EMSCRIPTEN__
    persistence_update(context);
#else
    for (int i = 0; i < 120; i++)
    {
        persistence_update(context);

        if (context->operation == PERSISTENCE_OPERATION_NONE && context->storage == NULL && !context->save_pending)
        {
            break;
        }

        if (context->storage != NULL && !SDL_StorageReady(context->storage))
        {
            SDL_Delay(1);
        }
    }

    persistence_close_storage(context);
#endif
}
