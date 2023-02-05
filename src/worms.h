#ifndef ROOTS_WORMS_H
#define ROOTS_WORMS_H

#include "raylib.h"
#include "world.h"

#define MAX_WORMS 20

typedef struct Worms
{
    Texture2D tex;
    Vector2 pos[MAX_WORMS];
    bool alive[MAX_WORMS];
} Worms;

void worms_init(Worms* worms);
void worms_spawm(Worms* worms, int x, int y);
void worms_update(Worms* worms, int last_scroll);
void worms_draw(Worms* worms);

#endif
