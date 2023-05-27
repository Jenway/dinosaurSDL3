#if !defined(__DATA_H__)
#define __DATA_H__

#include "./SDL2/SDL_stdinc.h"

#define DINO_POSTION_X 20
#define DINO_POSTION_Y 500
#define DINO_POSTION_W 200
#define DINO_POSTION_H 200
#define DINO_COLLISION_X 60
#define DINO_COLLISION_Y 540
#define DINO_COLLISION_W 100
#define DINO_COLLISION_H 150

#define DINO_JUMP_HEIGHT 200
#define DINO_JUMP_SPEED 10
#define DINO_JUMP_TIME 20
#define DINO_GRAVITY 10

#define DINO_FRAME_CHANGE_TIME 200
#define DINO_FRAME_DELTA_TIME 20
#define DINO_FRAME_COUNT 3

#define BACKGROUND_IMAGE_PATH "assets/bak.bmp"
#define DINO_IMAGE_PATH "assets/dino.png"
#define DINO_IMAGE_PATH2 "assets/dino2.png"
#define DINO_IMAGE_PATH3 "assets/dino3.png"
#define OBSTACLE_IMAGE_PATH "assets/obstacle.png"
#define DINO_PATH "assets/dino.png"

#define BACKGROUND_POSITION_X 0
#define BACKGROUND_POSITION_Y 0
#define BACKGROUND_POSITION_W 800
#define BACKGROUND_POSITION_H 300

#define ROAD_POSITION_X 0
#define ROAD_POSITION_Y 300
#define ROAD_POSITION_W 800
#define ROAD_POSITION_H 100

#define OBSTACLE_POSITION_X 800
#define OBSTACLE_POSITION_Y 300
#define OBSTACLE_POSITION_W 100
#define OBSTACLE_POSITION_H 100
#define OBSTACLE_SPEED 10

#define WINDOW_WIDTH 800

#define BACKGROUND_SPEED 15

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

// Uint32 timeCountPre = 0;
// Uint32 timeCountCur = 0;
// Uint32 FramePerSecond = 1000 / FPS;
// const Uint8* keyarr = NULL;

// int dinoRun[5] = { 1514, 1602, 1338, 1865,
//     1984 }; // 龙动画在资源中的位置 // run,run,steady,sit,sit
// int dinoJumpHeight = 200; // 数值参数
// int dinoStatus = 0; // 0 stable,1 running,2 jumpingOn ,3 jumpingOff,4 sitting,5
//                     // quikDown，6 DEAD
// int dinoRunningStatus = 0; // 龙动画阶段
// int roadStatus = 0; // 路图像位置
// int cloudStaatus = 800; // 云图像位置
// int plantStatus = 800;
// int gameStart = 1; // 游戏状态
// int sitCache = 0; // 爬行用
// int runCache = 0;
// int dinoAirCache = 0;
// int plantAttack[6];

#endif // __DATA_H__
