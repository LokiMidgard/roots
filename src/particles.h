#ifndef ROOTS_PARTICLES_H
#define ROOTS_PARTICLES_H

#include "raylib.h"

#define NUM_PARTS 100

typedef struct Particles {
    Vector2 pos[NUM_PARTS];
    Vector2 spd[NUM_PARTS];
    Vector2 acc[NUM_PARTS];
    float age[NUM_PARTS];
} Particles;

void particles_init(Particles* p);
void particles_emit(Particles* p, int num, int x, int y);
void particles_update(Particles* p);
void particles_draw(Particles* p);


#endif