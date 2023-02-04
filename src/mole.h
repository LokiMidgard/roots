#ifndef ROOTS_MOLE_H
#define ROOTS_MOLE_H

#include "raylib.h"
#include "sprite.h"

typedef struct Mole
{
    Sprite sprite;
    int points;
    float health;
    float speed;
    // bonuses have a value indicating the number of frames they have effect.
    float speedBonus;
    float stoneEaterBonus;
    
    Sound snd_dig;
    Sound snd_collide;
    Sound snd_explode;

    bool explode_req;
    float explode_time;
} Mole;

void mole_update(Mole *mole, Vector2 *movement);
void mole_init(Mole *mole, float x, float y);
void mole_draw(Mole* mole);
void mole_explode(Mole* mole);

#endif
