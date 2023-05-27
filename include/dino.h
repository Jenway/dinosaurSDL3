#if !defined(_DINO_H_)
#define _DINO_H_

#include <SDL_render.h>
#include <stdbool.h>

typedef struct dino {
    bool Alive;
    bool OnAir;
    bool OnCrawl;
    int y_velocity;
    SDL_Rect position;
    SDL_Rect collision_box;
    // frame
    int frame;
    // dino frame timer
    Uint32 frame_timer;
    // Texture
    SDL_Texture* texture[4];
} Dino;

bool Dino_Load(SDL_Renderer* renderer);
void Dino_Update();
void Dino_Draw(SDL_Renderer* renderer);
void Dino_Destroy();

void Dino_Init();

void Dino_Update_position();
void Dino_Update_Frame();
SDL_Rect Dino_Get_Collision();
bool Dino_Alive();
void Dino_Jump();
void Dino_Crawl();
void Dino_DeCrawl();
void Dino_Update_POS();
void dino_run();

#endif // _DINO_H_