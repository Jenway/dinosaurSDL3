
#if !defined(__OBSTACLE_H__)
#define __OBSTACLE_H__
#include "./SDL2/SDL_render.h"
#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"
#include <stdbool.h>

// there's 2 types of obstacles : plant and bird
// plant has 3 types : small, medium, large
// bird has 2 frame to change time to time
// how do we implement this?

// we can use enum to define the type of obstacle

enum obstacle_type {
    small_plant1,
    small_plant2,
    small_plant3,
    big_plant1,
    big_plant2,
    mix_plant,
    bird,
    OBSTACLE_COUNT
};

// we can use struct to define the obstacle

typedef struct obstacle {
    enum obstacle_type index;
    SDL_Rect position;
    SDL_Rect collision_box;
    int speed;
    SDL_Texture* texture[2];

    Uint32 FramePerSecond;
    int frame;
} Obstacle;

Obstacle* Obstacle_Create(int index, int Speed);

bool Obstacle_Load(struct obstacle* self, SDL_Renderer* renderer);
bool Obstacle_Collision(Obstacle* self, SDL_Rect dino);
void Obstacle_Init(struct obstacle* self);
void Obstacle_Update(struct obstacle* self);
void Obstacle_Draw(struct obstacle* self, SDL_Renderer* renderer);
void Obstacle_Destroy(struct obstacle* self);

#endif // __OBSTACLE_H__
