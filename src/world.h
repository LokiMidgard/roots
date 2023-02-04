#ifndef ROOTS_WORLD_H
#define ROOTS_WORLD_H

#include "raylib.h"
#include "mole.h"

typedef struct World {
    float speed;
    float pos_remainder;
    Image image;
    Color* bitmap;
    Texture2D screen_texture;
    int depth;
} World;

void world_init(World* world);
void world_update(World *world, Mole *mole);
void world_draw(World* world);

#endif