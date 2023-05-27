#if !defined(__SCORE_H__)
#define __SCORE_H__

#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_ttf.h"
#include <stdbool.h>

typedef struct score {
    SDL_Rect position;
    SDL_Texture* texture;
    SDL_Renderer* renderer;
    TTF_Font* font;
    SDL_Color color;
    int score;
} Score;

bool Score_Load(SDL_Renderer* renderer);
void Score_Update();
void Score_Draw(SDL_Renderer* renderer);
void Score_Destroy();

#endif // __SCORE_H__