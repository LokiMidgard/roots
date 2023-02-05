#ifndef ROOTS_SCREEN_H
#define ROOTS_SCREEN_H

#include "raylib.h"

typedef struct Screen {
    RenderTexture2D buffer;
} Screen;

void screen_init(Screen* screen);
void screen_draw(Screen* screen);

#endif