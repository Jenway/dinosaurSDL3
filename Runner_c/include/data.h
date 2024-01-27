#if !defined(__DATA_H__)
#define __DATA_H__

#include "./SDL2/SDL_stdinc.h"

#define DINO_POSTION_X 20
#define DINO_POSTION_Y 400
#define DINO_POSTION_W 100
#define DINO_POSTION_H 100
#define DINO_COLLISION_X 60
#define DINO_COLLISION_Y 540
#define DINO_COLLISION_W 100
#define DINO_COLLISION_H 150

#define DINO_JUMP_HEIGHT 200
#define DINO_JUMP_SPEED -980
#define DINO_FALL_SPEED 980
#define DINO_GRAVITY 40

#define DINO_FRAME_CHANGE_TIME 200
#define DINO_FRAME_DELTA_TIME 20
#define DINO_FRAME_COUNT 2

#define SPRITE_SHEET_PATH "assets/sprite.png"
// below are the sprite sheet coordinates
#define SPRITE_BACKGROUND_X 0
#define SPRITE_BACKGROUND_Y 0
#define SPRITE_BACKGROUND_W 800
#define SPRITE_BACKGROUND_H 300

#define SPRITE_DINO_RUN_1_X 0
#define SPRITE_DINO_RUN_1_Y 0
#define SPRITE_DINO_RUN_1_W 88
#define SPRITE_DINO_RUN_1_H 94

#define SPRITE_DINO_RUN_2_X 88
#define SPRITE_DINO_RUN_2_Y 0
#define SPRITE_DINO_RUN_2_W 88
#define SPRITE_DINO_RUN_2_H 94

#define LAYER_GROUND_PNG "assets/LAYER_GROUND.png"
#define BACKGROUND_IMAGE_PATH "assets/obstacle.png"
#define DINO_IMAGE_PATH "assets/dino.png"
#define DINO_IMAGE_PATH1 "assets/Run1.png"
#define DINO_IMAGE_PATH2 "assets/Run2.png"
#define DINO_IMAGE_PATH3 "assets/Sprint1.png"
#define DINO_IMAGE_PATH4 "assets/Sprint2.png"
#define OBSTACLE_IMAGE_PATH "assets/obstacle.png"

#define BACKGROUND_POSITION_X 0
#define BACKGROUND_POSITION_Y 0
#define BACKGROUND_POSITION_W 3200
#define BACKGROUND_POSITION_H 600

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
#define FPS 60
#define FRAMEPERSECOND 1000 / FPS

#define SITFPS 2
#define RUNFPS 2
#define PLANTHEIGHT 75
#define PLANTLENGTH 300
#define DINOFINALHEIGHT 350
#define DINOAIRTIME 1

#endif // __DATA_H__
