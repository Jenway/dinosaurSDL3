#include "../include/obstacle.h"
#include "../include/SDL2/SDL_image.h"
#include "../include/data.h"


Obstacle obstacle;

bool Obstacle_Load(SDL_Renderer* renderer)
{
    obstacle.texture = IMG_LoadTexture(renderer, OBSTACLE_IMAGE_PATH);
    if (!obstacle.texture) {
        SDL_Log("Failed to load obstacle texture");
        return false;
    }
    return true;
}

void Obstacle_Init()
{
    obstacle.position.x = OBSTACLE_POSITION_X;
    obstacle.position.y = OBSTACLE_POSITION_Y;
    obstacle.position.w = OBSTACLE_POSITION_W;
    obstacle.position.h = OBSTACLE_POSITION_H;
    obstacle.speed = OBSTACLE_SPEED;
}

void Obstacle_Update_position()
{
    obstacle.position.x -= obstacle.speed;
    if (obstacle.position.x <= -obstacle.position.w) {
        obstacle.position.x = WINDOW_WIDTH;
    }
}

void Obstacle_Draw(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, obstacle.texture, NULL, &obstacle.position);
}

void Obstacle_Destroy() { SDL_DestroyTexture(obstacle.texture); }

SDL_Rect Obstacle_Get_Collision() { return obstacle.position; }

bool Obstacle_Collision(SDL_Rect dino_collision)
{
    if (SDL_HasIntersection(&dino_collision, &obstacle.position)) {
        return true;
    }
    return false;
}

void Obstacle_Reset()
{
    obstacle.position.x = WINDOW_WIDTH;
    obstacle.position.y = OBSTACLE_POSITION_Y;
    obstacle.position.w = OBSTACLE_POSITION_W;
    obstacle.position.h = OBSTACLE_POSITION_H;
}
