
#if !defined(__OBSTACLE_H__)
#define __OBSTACLE_H__
#include "./SDL2/SDL_render.h"
#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"
#include <stdbool.h>


typedef struct obstacle {
    SDL_Rect position;
    int speed;
    SDL_Texture* texture;
    SDL_Rect offset;
    SDL_Rect cutset, cutRect;
    int dinoRun[5];
    int dinoJumpHeight;
    int dinoStatus;
    int dinoRunningStatus;
    int roadStatus;
    int cloudStaatus;
    int plantStatus;

    Uint32 FramePerSecond;
    const Uint8* keyarr;
} Obstacle;

bool Obstacle_Load(SDL_Renderer* renderer);
void Obstacle_Init();
void Obstacle_Update();
void Obstacle_Draw(SDL_Renderer* renderer);
void Obstacle_Destroy();

void Obstacle_DrawRoad();
void Obstacle_DrawCloud();
void Obstacle_DrawPlant();
void Obstacle_DrawDino();

#endif // __OBSTACLE_H__
