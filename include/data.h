#if !defined(__DATA_H__)
#define __DATA_H__


/* Default game width.*/
#define DEFAULT_WIDTH 600

/** Frames per second.*/
#define FPS 60

/** @const */
#define RESOURCE_POSTFIX 'offline-resources-'



#define DINO_POSTION_X 20
#define DINO_POSTION_Y 400
#define DINO_POSTION_W 100
#define DINO_POSTION_H 100
#define DINO_COLLISION_X 20
#define DINO_COLLISION_Y 400
#define DINO_COLLISION_W 100
#define DINO_COLLISION_H 100

#define DINO_JUMP_HEIGHT 200
#define DINO_JUMP_SPEED -980
#define DINO_FALL_SPEED 980
#define DINO_GRAVITY 40

#define DINO_FRAME_CHANGE_TIME 200
#define DINO_FRAME_DELTA_TIME 20

#define SPRITE_SHEET_PATH "assets/sprite.png"

#define BACKGROUND_IMAGE_PATH "assets/obstacle.png"

#define DINO_IMAGE_PATH1 "assets/Run1.png"
#define DINO_IMAGE_PATH2 "assets/Run2.png"
#define DINO_IMAGE_PATH3 "assets/Sprint1.png"
#define DINO_IMAGE_PATH4 "assets/Sprint2.png"
#define DINO_IMAGE_PATH5 "assets/Idle.png"

#define LAYER_GROUND_PNG "assets/LAYER_GROUND.png"
#define LAYER_CLOUD_PNG "assets/LAYER_CLOUD.png"
#define LAYER_MOON_PNG "assets/LAYER_MOON.png"
#define LAYER_STAR_PNG "assets/LAYER_STAR.png"

#define SMALL_PLANT1_PNG "assets/Cactus_SMALL1.png"
#define SMALL_PLANT2_PNG "assets/Cactus_SMALL2.png"
#define SMALL_PLANT3_PNG "assets/Cactus_SMALL3.png"
#define BIG_PLANT1_PNG "assets/Cactus_BIG1.png"
#define BIG_PLANT2_PNG "assets/Cactus_BIG2.png"
#define MIX_PLANT_PNG "assets/Cactus_MIX.png"

#define BIRD1_PNG "assets/Bird1.png"
#define BIRD2_PNG "assets/Bird2.png"

#define PLANT_POSITION_X 800
#define PLANT_POSITION_Y 300
#define PLANT_POSITION_W 100
#define PLANT_POSITION_H 100

#define BIRD_POSITION_X 800
#define BIRD_POSITION_Y 300
#define BIRD_POSITION_W 100
#define BIRD_POSITION_H 100

#define LAYER_GROUND_INDEX 0
#define LAYER_CLOUD_INDEX 1
#define LAYER_MOON_INDEX 2
#define LAYER_STAR_INDEX 3

#define LAYER_GROUND_SPEED 8
#define LAYER_CLOUD_SPEED 4
#define LAYER_MOON_SPEED 1
#define LAYER_STAR_SPEED 2

#define OBSTACLE_IMAGE_PATH "assets/obstacle.png"
#define DINO_PATH "assets/dino.png"

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
// #define OBSTACLE_COUNT 5
#define WIDTH 800
#define HEIGHT 300
#define FOOTHEIGHT 90
#define ROADSPEED 15 // 地面移动速度
#define CLOUDHEIGHT 100
#define FRAMEPERSECOND 1000 / FPS
#define OBSTACLE_WIDTH 100
#define SITFPS 2
#define RUNFPS 2
#define PLANTHEIGHT 75
#define PLANTLENGTH 300
#define DINOFINALHEIGHT 350
#define DINOAIRTIME 1

#endif // __DATA_H__
