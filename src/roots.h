#ifndef ROOTS_H
#define ROOTS_H

#include "raylib.h"
#include "world.h"

#define MAX_LEAFS 50 
#define MAX_ROOT_SEGMENTS 100

typedef struct RootSegment
{
    Vector2 start;
    Vector2 direction;
    float   length;
    float   max_length;
    float   grow_speed;
    float   start_radius;
    int     parent;
} RootSegment;

typedef struct Roots
{
    int number_of_leafs;
    int number_of_segments;
    int leafs[MAX_LEAFS];
    RootSegment segments[MAX_ROOT_SEGMENTS];
} Roots;

void roots_update(Roots *roots, World *world);

#endif
