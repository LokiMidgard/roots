#include "screen.h"
#include "config.h"

void screen_init(Screen* screen) {
    screen->buffer = LoadRenderTexture(WIDTH, HEIGHT);
    SetTextureFilter(screen->buffer.texture, TEXTURE_FILTER_TRILINEAR);
    screen->shader = LoadShader(0, "resources/shader/screen.fs");
}

void screen_update(Screen* screen) {
    if (IsKeyPressed(KEY_F)) {
        if (!IsWindowFullscreen()) {
            screen->oldWidth = GetScreenWidth();
            screen->oldHeight = GetScreenHeight();
            int mon = GetCurrentMonitor();
            SetWindowSize(GetMonitorWidth(mon), GetMonitorHeight(mon));
            ToggleFullscreen();
        } else {
            ToggleFullscreen();
            SetWindowSize(screen->oldWidth, screen->oldHeight);
        }
    }
}

void screen_draw(Screen* screen) {
    Rectangle srcRect = { 0, 0, WIDTH, -HEIGHT };
    Rectangle dstRect = {0, 0, GetScreenWidth(), GetScreenHeight()};
    Vector2 origin = {0, 0};
    BeginShaderMode(screen->shader);
    DrawTexturePro(screen->buffer.texture, srcRect, dstRect, origin, 0.0f, WHITE);
    EndShaderMode();
}
