#include <stdbool.h>
#include "../lib/SDL2/SDL_rect.h"

typedef struct dino
{
    bool Alive;
    bool OnJump;
    bool OnCrawl;
    int speed;
    SDL_Rect position;
    SDL_Rect collision_box;
} Dino;

void Dino_Init();
SDL_Rect Dino_Get_Collision();
bool Dino_Alive();
void Dino_Jump();
void Dino_Update_POS();