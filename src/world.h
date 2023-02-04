#ifndef ROOTS_WORLD_H
#define ROOTS_WORLD_H

#include "raylib.h"
#include "mole.h"

typedef enum TerrainType
{
    EARTH,
    TUNNEL,
    STONE,
    ROOT,
    ROOT_TIP,
    ROOT_KNOT,
    EMERALD,
    QUICK_STONE,
    DIG_STONE,
    SAND,
    TerrainTypeSize,
} TerrainType;

typedef struct World
{
    float speed;
    float pos_remainder;
    int number_of_images;
    Image images[10];
    Color* current_bitmap;
    Color* next_bitmap;
    Texture2D screen_texture;
    int depth;
    int leftSpeed;
    int centerSpeed;
    int rightSpeed;
} World;

typedef struct Dig
{
    int types[TerrainTypeSize];
} Dig;

void world_init(World *world);
void world_update(World *world, Mole *mole);
void world_draw(World *world);

#endif
