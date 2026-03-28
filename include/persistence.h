#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include <stdbool.h>
#include <stdint.h>

#include <SDL3/SDL_storage.h>

typedef enum persistence_operation
{
    PERSISTENCE_OPERATION_NONE = 0,
    PERSISTENCE_OPERATION_LOAD,
    PERSISTENCE_OPERATION_SAVE,
} persistence_operation;

typedef struct persistence_context
{
    SDL_Storage *storage;
    persistence_operation operation;
    bool load_completed;
    bool loaded_value_available;
    uint32_t loaded_high_score;
    bool save_pending;
    uint32_t pending_high_score;
    bool storage_error_logged;
} persistence_context;

void persistence_init(persistence_context *context);
void persistence_update(persistence_context *context);
bool persistence_consume_loaded_high_score(persistence_context *context, uint32_t *high_score);
void persistence_request_high_score_save(persistence_context *context, uint32_t high_score);
void persistence_shutdown(persistence_context *context);

#endif
