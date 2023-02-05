#include "config.h"
#include "hud.h"

void hud_init(Hud* hud, int *inventory) {
    int size = 128;
    int border = 16;
    hud->tex_ninepatch = LoadTexture(TextFormat("resources/ninepatch_%i_%i.png", size, border));
    hud->np_info.source = (Rectangle){ 0.0f, 0.0f, size, size },
    hud->np_info.left = border;
    hud->np_info.top = border;
    hud->np_info.right = border;
    hud->np_info.bottom = border;
    hud->np_info.layout = NPATCH_NINE_PATCH;
    hud->inventory = inventory;
}

void hud_update(Hud* hud) {
}

void hud_draw(Hud* hud, Stuff *stuff) {
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

    for(int type = 0; type < st_size; ++type)
    {
        float left_offset = 30.0f;
        float top_offset  = 32.0f;
        float offset      = 30.0f;
        int count = hud->inventory[type];
        int source_start_x = type * 8;
        Rectangle srcRect = {source_start_x, 0, 8, 8};
        Rectangle dstRectStuff = {
            dstRect.x + left_offset,
            dstRect.y + type*offset + top_offset,
            16,
            16,
        };
        if (type >= 5)
        {
            dstRectStuff.x += 70;
            dstRectStuff.y -= 5*offset;
        }

        Vector2 origin = {0, 0};
        DrawTexturePro(stuff->tex, srcRect, dstRectStuff, origin, 0, WHITE);
        char text[16] = {0};
        snprintf(text, 16, "%2d", count);
        dstRectStuff.x += 20.0f;
        dstRectStuff.y -= 2.0f;
        DrawText(text, dstRectStuff.x, dstRectStuff.y, 20, BLACK);
    }

}
