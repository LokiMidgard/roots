#include "config.h"
#include "hud.h"

void hud_init(Hud* hud) {
    hud->tex_ninepatch = LoadTexture("resources/ninepatch.png");
    hud->np_info.source = (Rectangle){ 0.0f, 0.0f, 64.0f, 64.0f },
    hud->np_info.left = 6;
    hud->np_info.top = 6;
    hud->np_info.right = 6;
    hud->np_info.bottom = 6;
    hud->np_info.layout = NPATCH_NINE_PATCH;
}

void hud_update(Hud* hud) {
}

void hud_draw(Hud* hud) {
    int x = 4;
    int y = 4;
    int w = 200;
    int h = 200;

    Rectangle dstRect = { x, y, w, h };
    Vector2 origin = { 0, 0 };
    DrawTextureNPatch(hud->tex_ninepatch, hud->np_info, dstRect, origin, 0.0f, WHITE);
    DrawText(hud->debug_text, dstRect.x + hud->np_info.left + 2, dstRect.y + hud->np_info.top + 2, 20, BLACK);

    DrawFPS(WIDTH/2, 10);
}
