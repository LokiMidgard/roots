#include "particles.h"

void init(Particles* p, int i, int x, int y) {
        p->pos[i].x = x;
        p->pos[i].y = y;
        p->spd[i].x = 0.03 * (rand() % 100 - 50);
        p->spd[i].y = -0.01 * (rand() % 100);
        p->acc[i].x = 0;
        p->acc[i].y = 0.1f;
        p->age[i] = 0;
}

void particles_init(Particles* p) {
    for(int i=0; i<NUM_PARTS; ++i) {
        init(p, i, 0, 0);
    }
}

void particles_emit(Particles* p, int num, int x, int y) {
    for (int i=0; i<NUM_PARTS; ++i) {
        if (p->age[i] == 0) {
            init(p, i, x, y);
            p->age[i] = 15;
            --num;
        }
        if (num <= 0)
            break;
    }
}

void particles_update(Particles* p) {
    for(int i=0; i<NUM_PARTS; ++i) {
        if (p->age[i] > 0) {
            p->spd[i].x += p->acc[i].x;
            p->spd[i].y += p->acc[i].y;
            p->pos[i].x += p->spd[i].x;
            p->pos[i].y += p->spd[i].y;
            p->age[i] -= 1;

            if (p->age[i] <= 0) {
                init(p, i, 0, 0);
            }
        }
    }
}

void particles_draw(Particles* p) {
    float factorX = GetScreenWidth() / (float)WIDTH;
    float factorY = GetScreenHeight() / (float)HEIGHT;
    for(int i=0; i<NUM_PARTS; ++i) {
        if (p->age[i] >= 0) {
            DrawCircle(p->pos[i].x * factorX, p->pos[i].y * factorY, 2, DARKBROWN);
        }
    }
}
