#include "screen.h"
#include "config.h"

void screen_init(Screen* screen) {
    screen->buffer = LoadRenderTexture(WIDTH, HEIGHT);
    SetTextureFilter(screen->buffer.texture, TEXTURE_FILTER_POINT);
}

void screen_draw(Screen* screen) {
    Rectangle srcRect = { 0, 0, WIDTH, -HEIGHT };
    Rectangle dstRect = {0, 0, GetScreenWidth(), GetScreenHeight()};
    Vector2 origin = {0, 0};
    DrawTexturePro(screen->buffer.texture, srcRect, dstRect, origin, 0.0f, WHITE);
}
