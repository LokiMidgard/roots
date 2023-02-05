#ifndef ROOTS_HUD_H
#define ROOTS_HUD_H

#include "raylib.h"
#include "stuff.h"
#include "inventory.h"

typedef struct Hud {
    Texture2D tex_ninepatch;
    NPatchInfo np_info;
    char debug_text[256];
    Inventory* inventory;
} Hud;

void hud_init(Hud* hud, Inventory* inventory);
void hud_update(Hud* hud);
void hud_draw(Hud* hud, Stuff *stuff);

#endif
