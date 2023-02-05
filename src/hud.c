#include "config.h"
#include "hud.h"

void hud_init(Hud* hud) {
    int size = 128;
    int border = 16;
    hud->tex_ninepatch = LoadTexture(TextFormat("resources/ninepatch_%i_%i.png", size, border));
    hud->np_info.source = (Rectangle){ 0.0f, 0.0f, size, size },
    hud->np_info.left = border;
    hud->np_info.top = border;
    hud->np_info.right = border;
    hud->np_info.bottom = border;
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
    // debug text
    DrawTextureNPatch(hud->tex_ninepatch, hud->np_info, dstRect, origin, 0.0f, WHITE);
    DrawText(hud->debug_text, dstRect.x + hud->np_info.left + 2, dstRect.y + hud->np_info.top + 2, 20, BLACK);

    // fps
    DrawFPS(WIDTH/2, 10);

    // items
    dstRect.x = GetScreenWidth() -x - w;
    DrawTextureNPatch(hud->tex_ninepatch, hud->np_info, dstRect, origin, 0.0f, WHITE);

}
