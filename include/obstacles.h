#ifndef OBSTACLES_H
#define OBSTACLES_H

#include <stddef.h>

#define MAX_OBSTACLE_COLLISION_BOXES 5
#define MAX_OBSTACLE_Y_POSITIONS 3

typedef struct collision_box
{
    int x;
    int y;
    int width;
    int height;
} collision_box;

typedef enum obstacle_type
{
    OBSTACLE_TYPE_SMALL_CACTUS_1 = 0,
    OBSTACLE_TYPE_SMALL_CACTUS_2,
    OBSTACLE_TYPE_SMALL_CACTUS_3,
    OBSTACLE_TYPE_TALL_CACTUS_1,
    OBSTACLE_TYPE_TALL_CACTUS_2,
    OBSTACLE_TYPE_TALL_CACTUS_3,
    OBSTACLE_TYPE_PTERODACTYL,
    OBSTACLE_TYPE_COUNT,
} obstacle_type;

typedef struct obstacle_definition
{
    int sprite_x;
    int sprite_y;
    int sprite_width;
    int sprite_height;
    int min_gap;
    int min_scroll_speed;
    size_t y_position_count;
    int y_positions[MAX_OBSTACLE_Y_POSITIONS];
    size_t collision_box_count;
    collision_box collision_boxes[MAX_OBSTACLE_COLLISION_BOXES];
    int num_frames;
    int frame_ms;
    float speed_offset;
} obstacle_definition;

const obstacle_definition *obstacle_get_definition(obstacle_type type);
size_t obstacle_definition_count(void);

#endif
