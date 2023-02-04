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

#define TERRA_EARTH (BROWN)
#define TERRA_TUNEL (DARKBROWN)
#define TERRA_STONE (GRAY)
#define TERRA_ROOT (BLACK)
#define TERRA_EMERALD (GREEN)
#define TERRA_QUICK_STONE (BLUE)
#define TERRA_DIG_STONE (RED)

#define NUM_SEEDS (30)

#define IS_COLOR(pos, col) (pos->r == col.r && pos->g == col.g && pos->b == col.b)

#endif
