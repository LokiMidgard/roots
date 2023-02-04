#ifndef ROOTS_MOLE_H
#define ROOTS_MOLE_H

#include "raylib.h"
#include "sprite.h"

typedef struct Mole {
    Sprite sprite;
    float points;
    float health;
    float speed;
    // bonuses have a valu indicating the number of frames they have effect.
    float speedBonus;
    float stoneEaterBonus;
    Sound snd_dig;
    
} Mole;


void mole_update(Mole *mole, Vector2 *movement, Color *bitmap);
void mole_init(Mole *mole, float x, float y);


#endif