#include "config.h"
#include "hud.h"

#define _WIDTH WIDTH        // GetScreenWidth()
#define _HEIGHT HEIGHT      // GetSCreenHeight()

void hud_init(Hud* hud, Inventory* inventory) {
    int size = 128;
    int border = 16;

    hud->tex_ninepatch = LoadTexture(TextFormat("resources/ninepatch/ninepatch_%i_%i.png", size, border));
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
    int w = _WIDTH - 16;
    int h = 40;

    int x = 8;
    int y = _HEIGHT -8 - h;

    Rectangle dstRect = { x, y, w, h };
    Vector2 origin = { 0, 0 };

    // debug text
    if (TextLength(hud->debug_text) > 0) {
        int count = 0;
        int gap = 16;
        const char** texts = TextSplit((const char*)hud->debug_text, '\n', &count);
        float width = dstRect.width / count;
        dstRect.width = width - gap;
        for(int i=0; i<count; ++i) {
            dstRect.x = 8 + (gap * 0.5) + (i*width);
            DrawTextureNPatch(hud->tex_ninepatch, hud->np_info, dstRect, origin, 0.0f, WHITE);
            DrawText(texts[i], dstRect.x + hud->np_info.left + 4, dstRect.y + hud->np_info.top - 1, 12, BLACK);
        }
    }

    // fps
    // DrawFPS(WIDTH/2, 10);

    // items
    float top_right_hud_size = 90.0f;
    Rectangle top_right_hud =
    {
        _WIDTH - top_right_hud_size - 16,
        16,
        top_right_hud_size,
        top_right_hud_size
    };
    Color circle_background_color = {120, 120, 120, 200};
    DrawCircle(top_right_hud.x + top_right_hud_size * 0.5f, 
               top_right_hud.y + top_right_hud_size * 0.5f,
               top_right_hud_size * 0.5f,
               circle_background_color);

    float top_offset  = 16.0f;
    float left_offset = 20.0f;
    Vector2 offset = {14.0f, 10.0f};
    Vector2 item_positions[4] =
    {
        {
            top_right_hud.x + top_right_hud.width*0.5f - offset.x + 3.0f,
            top_right_hud.y + top_offset - offset.y
        },
        {
            top_right_hud.x + left_offset - offset.x,
            top_right_hud.y + top_right_hud.height*0.5f - offset.y
        },
        {
            top_right_hud.x + top_right_hud.width*0.5f - offset.x + 3.0f,
            top_right_hud.y + top_right_hud.height - top_offset - offset.y
        },
        {
            top_right_hud.x - left_offset + top_right_hud.width - offset.x,
            top_right_hud.y + top_right_hud.height*0.5f - offset.y
        },
    };

    for(int type = 0; type < 4; ++type)
    {
        int count = hud->inventory->pickups[type];
        int source_start_x = type * 8;
        Rectangle srcRect = {source_start_x, 0, 8, 8};
        Rectangle target =
        {
            item_positions[type].x,
            item_positions[type].y,
            16,
            16,
        };
        Vector2 origin = {0, 0};
        DrawTexturePro(stuff->tex, srcRect, target, origin, 0, WHITE);
        char text[16] = {0};
        snprintf(text, 16, "%2d", count);
        target.x += 10.0f;
        target.y -= 0.0f;
        DrawText(text, target.x, target.y, 20, BLACK);
    }
}
