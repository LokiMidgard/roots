#ifndef ROOTS_WORLD_H
#define ROOTS_WORLD_H

#include "raylib.h"
#include "mole.h"

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

void world_init(World *world);
void world_update(World *world, Mole *mole);
void world_draw(World *world);

#endif
