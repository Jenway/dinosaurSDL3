#include "../lib/SDL2/SDL.h"
#include "../lib/SDL2/SDL_keyboard.h"
#include "../lib/SDL2/SDL_image.h"
#include <stdio.h>
#include <math.h>

#define DINO_PATH "resource/dino.png"
#define BAKIMG_PATH "resource/bak.png"
#define WIDTH 800
#define HEIGHT 300
#define FOOTHEIGHT 90
#define ROADSPEED 15 // 地面移动速度
#define CLOUDHEIGHT 100
#define FPS 30
#define SITFPS 2
#define RUNFPS 2
#define PLANTHEIGHT 75
#define PLANTLENGTH 300
#define DINOFINALHEIGHT 350
#define DINOAIRTIME 1

Uint32 timeCountPre = 0;
Uint32 timeCountCur = 0;
Uint32 FramePerSecond = 1000 / FPS;
const Uint8 *keyarr = NULL;

int dinoRun[5] = {1514, 1602, 1338, 1865, 1984}; // 龙动画在资源中的位置 // run,run,steady,sit,sit
int dinoJumpHeight = 200;                        // 数值参数
int dinoStatus = 0;                              // 0 stable,1 running,2 jumpingOn ,3 jumpingOff,4 sitting,5 quikDown，6 DEAD
int dinoRunningStatus = 0;                       // 龙动画阶段
int roadStatus = 0;                              // 路图像位置
int cloudStaatus = 800;                          // 云图像位置
int plantStatus = 800;
int gameStart = 1; // 游戏状态
int sitCache = 0;  // 爬行用
int runCache = 0;
int dinoAirCache = 0;
int plantAttack[6];


SDL_Window *window = NULL; // Declare a pointer
SDL_Renderer *renderer = NULL;
SDL_Surface *background = NULL;
SDL_Surface *message = NULL;
SDL_Surface *surface = NULL;
SDL_Texture *texture = NULL;
// 新建一个临时的矩形来保存偏移量
SDL_Rect offset;
// 切割原图用的矩形
SDL_Rect cutset, cutRect;

// 加载图片
SDL_Surface *LoadImage(const char *file);
void apply_surface(int x, int y, SDL_Surface *source, SDL_Surface *destination);
void apply_dino(int x, int y, int x2, int y2, int w, int h, SDL_Surface *source, SDL_Surface *destination);
void dino_run();
void drawRoad();
void drawCloud();
void drawPlant();
void initBak();
void paintevent();
void dinoJump();
void dinoQuickdown();
void gameEnd();
void judge();