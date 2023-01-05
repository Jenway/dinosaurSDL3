#include "SDL2/SDL.h"
#include <stdio.h>

#define DINO_PATH "Resources/dino.bmp"
#define BAKIMG_PATH "Resources/bak.bmp"
#define WIDTH 800
#define HEIGHT 300
#define FOOTHEIGHT 90
#define ROADSPEED 10 // 地面移动速度
#define CLOUDHEIGHT 100

int dinoRun[3] = {1514, 1602, 1338}; // 龙动画在资源中的位置
int dinoJumpHeight = 200; // 数值参数
int dinoStatus = 0; // 0 stable,1 running,2 jumpingOn ,3 jumpingOff,4 sitting
int dinoRunningStatus = 0; // 龙动画阶段
int roadStatus = 0;//路图像位置
int cloudStaatus = 800 ; // 云图像位置
SDL_Window *window = NULL; // Declare a pointer
SDL_Renderer *renderer = NULL;
SDL_Surface *background = NULL;
SDL_Surface *message = NULL;
SDL_Surface *surface = NULL;
SDL_Texture *texture =NULL;
    // 新建一个临时的矩形来保存偏移量
    SDL_Rect offset;
    // 切割原图用的矩形
    SDL_Rect cutset,cutRect;

// 加载图片
SDL_Surface *LoadImage(const char *file);
void apply_surface(int x, int y, SDL_Surface *source, SDL_Surface *destination);
void apply_dino(int x, int y, int x2, int y2, int w, int h, SDL_Surface *source, SDL_Surface *destination);
void dino_run();
void drawRoad();
void drawCloud();
void initBak();
void paintevent();
void dinoJump();