#ifndef ROOTS_H
#define ROOTS_H

#include "raylib.h"
//#include "world.h"

#define MAX_ROOT_PATHS 100
#define ROOT_PATH_SIZE 10

typedef struct RootPath
{
    Vector2 nodes[ROOT_PATH_SIZE];
    Vector2 growth_direction;
    int     first_node;
    float   tip_size;
    float   tip_size_target;
    float   growth_speed;
} RootPath;

typedef struct Roots
{
    int number_of_paths;
    RootPath paths[MAX_ROOT_PATHS];
} Roots;

void roots_update(Roots *roots, World *world);

#endif
