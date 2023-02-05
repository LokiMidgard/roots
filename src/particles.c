#include "particles.h"

void init(Particles* p, int i, int x, int y, Color color) {
        p->pos[i].x = x + (0.1f * (rand() % 100)) - 5;
        p->pos[i].y = y + (0.1f * (rand() % 100)) - 5;
        p->spd[i].x = 0.03 * (rand() % 100 - 50);
        p->spd[i].y = -0.01 * (rand() % 100);
        p->acc[i].x = 0;
        p->acc[i].y = 0.1f;
        p->age[i] = 0;
        p->color[i] = color;
}

void particles_init(Particles* p) {
    for(int i=0; i<NUM_PARTS; ++i) {
        init(p, i, 0, 0, PINK);
    }
}

void particles_emit(Particles* p, int num, int x, int y, Color color) {
    Color c = { (unsigned char)((int)color.r * 0.5f), (unsigned char)((int)color.g * 0.5f), (unsigned char)((int)color.b * 0.5f), 255};

    for (int i=0; i<NUM_PARTS; ++i) {
        if (p->age[i] == 0) {
            init(p, i, x, y, c);
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
                init(p, i, 0, 0, PINK);
            }
        }
    }
}

void particles_draw(Particles* p) {
    float factorX = GetScreenWidth() / (float)WIDTH;
    float factorY = GetScreenHeight() / (float)HEIGHT;
    for(int i=0; i<NUM_PARTS; ++i) {
        if (p->age[i] > 0) {
            DrawCircle(p->pos[i].x * factorX, p->pos[i].y * factorY, 2, p->color[i]);
        }
    }
}
