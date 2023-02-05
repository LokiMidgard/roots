#ifndef ROOTS_MOLE_H
#define ROOTS_MOLE_H

#include "raylib.h"
#include "sprite.h"
#include "particles.h"

typedef struct Mole
{
    // graphics
    Sprite sprite;
    
    // physics
    float speed;

    // game logic
    int points;
    float health;
    // bonuses have a value indicating the number of frames they have effect.
    float speedBonus;
    float stoneEaterBonus;

    // sound
    Sound snd_dig;
    Sound snd_dig_earth;
    Sound snd_dig_stone;
    Sound snd_collide;
    Sound snd_explode;
    Sound snd_hurt;

    // fx
    float explode_time;
    Particles part_dig;
} Mole;

void mole_update(Mole *mole, Vector2 movement);
void mole_init(Mole *mole, float x, float y);
void mole_draw(Mole* mole);
void mole_explode(Mole* mole);

#endif
