#include "raymath.h"
#include "particles.h"
#include "config.h"

void init(Particles* p, int i, int x, int y, Color color, float boost) {
        float spd = 0.03 * (rand() % 100 - 50) * boost;
        float angle = rand() %360;
        Vector2 vSpd = Vector2Rotate((Vector2){spd, 0}, angle);
        p->pos[i].x = x;
        p->pos[i].y = y;
        p->spd[i].x = vSpd.x;
        p->spd[i].y = -vSpd.y;
        p->acc[i].x = 0;
        p->acc[i].y = 0.1f;
        p->age[i] = 0;
        p->color[i] = color;
}

void particles_init(Particles* p) {
    for(int i=0; i<NUM_PARTS; ++i) {
        init(p, i, 0, 0, PINK, 1);
    }
}

void particles_emit(Particles* p, int num, int x, int y, Color color, float boost) {
    for (int i=0; i<NUM_PARTS; ++i) {
        if (p->age[i] == 0) {
            float factor = 0.4f + (rand() % 50) / 100.0f;
            Color c = { (unsigned char)((int)color.r * factor), (unsigned char)((int)color.g * factor), (unsigned char)((int)color.b * factor), 255};

            init(p, i, x, y, c, boost);
            p->age[i] = 15 * boost;
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
                init(p, i, 0, 0, PINK, 1);
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
