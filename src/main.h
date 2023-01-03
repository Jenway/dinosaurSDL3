#include "SDL2/SDL.h"
#include <stdio.h>

#define POINTS_COUNT 4
#define DINO_PATH "Resources/dino.bmp"
#define BAKIMG_PATH "Resources/bak.bmp"
#define WIDTH 1600
#define HEIGHT 900

int dinoRun[2] = {1514, 1602};
int dinoJumpHeight =200;
int dinoStatus;
SDL_Window *window = NULL; // Declare a pointer
SDL_Renderer *renderer = NULL;
SDL_Surface *background = NULL;
SDL_Surface *message = NULL;
SDL_Surface *surface=NULL;

// 加载图片
SDL_Surface *LoadImage(const char *file);
void apply_surface(int x, int y, SDL_Surface *source, SDL_Surface *destination);
void apply_dino(int x, int y, int x2, int y2, int w, int h, SDL_Surface *source, SDL_Surface *destination);
void dino_run();
void drawRoad();
void initBak();
void paintevent();
