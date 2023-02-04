#ifndef ROOTS_MOLE_H
#define ROOTS_MOLE_H

#include "raylib.h"
#include "sprite.h"

typedef struct Mole {
    Sprite sprite;
    Sound snd_dig;
    Sound snd_collide;
} Mole;

void mole_update(Mole *mole, Vector2 *movement, Color *bitmap);

#endif