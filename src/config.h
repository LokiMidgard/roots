#ifndef ROOTS_CONFIG_H
#define ROOTS_CONFIG_H

#include "raylib.h"

#define FPS (60)
#define WIDTH (960)
#define HEIGHT (540)
#define SCALE (1.5f)
#define WINDOW_WIDTH ((int)(WIDTH*SCALE))
#define WINDOW_HEIGHT ((int)(HEIGHT*SCALE))
#define POS(x, y) ((y) * (WIDTH) + (x))

#define TERRA_EARTH         CLITERAL(Color){ 127, 106,  79, 255 }   // RAYLIB BROWN
#define TERRA_TUNEL         CLITERAL(Color){  76,  63,  47, 255 }   // RAYLIB DARKBROWN
#define TERRA_STONE         CLITERAL(Color){ 130, 130, 130, 255 }   // RAYLIB GRAY
#define TERRA_ROOT          CLITERAL(Color){   0,   0,   0, 255 }   // BLACK
#define TERRA_ROOT_TIP      CLITERAL(Color){   1,   0,   0, 255 }   // 1-RED
#define TERRA_ROOT_KNOT     CLITERAL(Color){   2,   0,   0, 255 }   // 2-RED
#define TERRA_EMERALD       CLITERAL(Color){   3, 228,  48, 255 }   // 3-RED
#define TERRA_QUICK_STONE   CLITERAL(Color){   3, 121, 241, 255 }   // ???
#define TERRA_DIG_STONE     CLITERAL(Color){ 230,  41,  55, 255 }   // ???
#define TERRA_SAND          CLITERAL(Color){ 237, 214,  92, 255 }   //
#define TERRA_LAVA          CLITERAL(Color){ 255,   0,   0, 255 }   // RED

#define NUM_SEEDS (30)

#define MOLE_MAX_HEALTH (100)
#define INV_MAX_ITEMS (9)

#endif

