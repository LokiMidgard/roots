#ifndef ROOTS_CONFIG_H
#define ROOTS_CONFIG_H

#define FPS (60)
#define WIDTH (960)
#define HEIGHT (540)
#define SCALE (1.5f)
#define WINDOW_WIDTH ((int)(WIDTH*SCALE))
#define WINDOW_HEIGHT ((int)(HEIGHT*SCALE))
#define POS(x, y) ((y) * (WIDTH) + (x))

#define IS_COLOR(pos, col) (pos->r == col.r && pos->g == col.g && pos->b == col.b)

#endif