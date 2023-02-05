#ifndef ROOTS_HUD_H
#define ROOTS_HUD_H

#include "raylib.h"
#include "stuff.h"

typedef struct Hud {
    Texture2D tex_ninepatch;
    NPatchInfo np_info;
    char debug_text[256];
    int *inventory;
} Hud;

void hud_init(Hud* hud, int *inventory);
void hud_update(Hud* hud);
void hud_draw(Hud* hud, Stuff *stuff);

#endif
