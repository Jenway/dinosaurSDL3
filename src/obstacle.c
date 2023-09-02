#include "../include/obstacle.h"
#include "../include/SDL2/SDL_image.h"
#include "../include/data.h"
#include "SDL_log.h"
#include <stdbool.h>

Obstacle* Obstacle_Create(int index, int Speed)
{
    Obstacle* obstacle = malloc(sizeof(Obstacle));
    obstacle->index = index;
    obstacle->speed = Speed;
    Obstacle_Init(obstacle);
    return obstacle;
}


bool Obstacle_Collision(Obstacle* self, SDL_Rect dino)
{
    SDL_Rect obstacle_rect = self->position;
    return SDL_HasIntersection(&dino, &obstacle_rect);
}

void Obstacle_Init(struct obstacle* self)
{
    switch (self->index) {
    case small_plant1:
        self->position.x = PLANT_POSITION_X;
        self->position.y = PLANT_POSITION_Y;
        self->position.w = PLANT_POSITION_W;
        self->position.h = PLANT_POSITION_H;
        break;
    case small_plant2:
        self->position.x = PLANT_POSITION_X;
        self->position.y = PLANT_POSITION_Y;
        self->position.w = PLANT_POSITION_W;
        self->position.h = PLANT_POSITION_W;
        break;
    case small_plant3:
        self->position.x = PLANT_POSITION_X;
        self->position.y = PLANT_POSITION_Y;
        self->position.w = PLANT_POSITION_W;
        self->position.h = PLANT_POSITION_H;
        break;
    case big_plant1:
        self->position.x = PLANT_POSITION_X;
        self->position.y = PLANT_POSITION_Y;
        self->position.w = PLANT_POSITION_W;
        self->position.h = PLANT_POSITION_H;
        break;
    case big_plant2:
        self->position.x = PLANT_POSITION_X;
        self->position.y = PLANT_POSITION_Y;
        self->position.w = PLANT_POSITION_W;
        self->position.h = PLANT_POSITION_H;
        break;
    case mix_plant:
        self->position.x = PLANT_POSITION_X;
        self->position.y = PLANT_POSITION_Y;
        self->position.w = PLANT_POSITION_W;
        self->position.h = PLANT_POSITION_H;
        break;
    case bird:
        self->position.x = BIRD_POSITION_X;
        self->position.y = BIRD_POSITION_Y;
        self->position.w = BIRD_POSITION_W;
        self->position.h = BIRD_POSITION_H;
        break;
    }
}

bool Obstacle_Load(struct obstacle* self, SDL_Renderer* renderer)
{
    switch (self->index) {
    case small_plant1:
        self->texture[0] = IMG_LoadTexture(renderer, SMALL_PLANT1_PNG);
        break;
    case small_plant2:
        self->texture[0] = IMG_LoadTexture(renderer, SMALL_PLANT2_PNG);
        break;
    case small_plant3:
        self->texture[0] = IMG_LoadTexture(renderer, SMALL_PLANT3_PNG);
        break;
    case big_plant1:
        self->texture[0] = IMG_LoadTexture(renderer, BIG_PLANT1_PNG);
        break;
    case big_plant2:
        self->texture[0] = IMG_LoadTexture(renderer, BIG_PLANT2_PNG);
        break;
    case mix_plant:
        self->texture[0] = IMG_LoadTexture(renderer, MIX_PLANT_PNG);
        break;
    case bird:
        self->texture[0] = IMG_LoadTexture(renderer, BIRD1_PNG);
        self->texture[1] = IMG_LoadTexture(renderer, BIRD2_PNG);
        break;
    default:
        break;
    }
    if (!self->texture[0]) {
        SDL_Log("Failed to create obstacle texture: %s", SDL_GetError());
        return false;
    }
    return true;
}

void Obstacle_Draw(struct obstacle* self, SDL_Renderer* renderer)
{
    for (int i = 0; i < OBSTACLE_COUNT; i++) {
        if (self->texture[1] != NULL) {
            SDL_RenderCopy(renderer, self->texture[1], NULL, &self->position);
        }
    }
}

void Obstacle_Update(struct obstacle* self)
{
    self->position.x -= self->speed;
    if (self->position.x <= -OBSTACLE_WIDTH) {
        self->position.x = 100;
    }
    // SDL_Log("Obstacle position: %d", self->position.x);
}

void Obstacle_Destroy(struct obstacle* self)
{
    SDL_DestroyTexture(self->texture[0]);
    if (self->texture[1] != NULL) {
        SDL_DestroyTexture(self->texture[1]);
    }
    // free(self);
}