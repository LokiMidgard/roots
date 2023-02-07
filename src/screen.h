#ifndef ROOTS_SCREEN_H
#define ROOTS_SCREEN_H

#include "raylib.h"

typedef struct Screen {
    RenderTexture2D buffer;
    Shader shader;
    int oldWidth;
    int oldHeight;
} Screen;

void screen_init(Screen* screen);
void screen_update(Screen* screen);
void screen_draw(Screen* screen);

#endif

