#include "config.h"
#include "input.h"

int g_device = INPUT_KEYBOARD;
Sprite* g_sprite = (Sprite*)0;
int g_mouse_deadzone = 20;

void input_set_device(int device) { g_device = device; }
void input_set_mouse_center(Sprite* sprite) { g_sprite = sprite; }
void input_set_mouse_deadzone(int deadzone) { g_mouse_deadzone = deadzone; }

Vector2 input_get_dir() {
    Vector2 dir = { 0, 0 };

    switch(g_device) {
        case INPUT_KEYBOARD:
            if (IsKeyDown(KEY_RIGHT)) dir.x = 1;
            if (IsKeyDown(KEY_LEFT)) dir.x = -1;
            if (IsKeyDown(KEY_UP)) dir.y = -1;
            if (IsKeyDown(KEY_DOWN)) dir.y = 1;
            break;
    
        case INPUT_MOUSE:
            dir = GetMousePosition();
            dir.x *= WIDTH/(float)GetScreenWidth();
            dir.y *= HEIGHT/(float)GetScreenHeight();
            dir.x -= g_sprite ? g_sprite->position.x : (WIDTH >> 1);
            dir.y -= g_sprite ? g_sprite->position.y : (HEIGHT >> 1);

            if (abs(dir.x) < g_mouse_deadzone) dir.x = 0;
            if (abs(dir.y) < g_mouse_deadzone) dir.y = 0;
            break;

        case INPUT_GAMEPAD:
        break;
    }

    return Vector2Normalize(dir);
}

bool input_is_button_pressed(int button) {
    switch(g_device) {
        case INPUT_KEYBOARD:
            switch(button) {
                case 0: return IsKeyDown(KBD_BUTTON_0);
                case 1: return IsKeyDown(KBD_BUTTON_1);
            }
            break;
        case INPUT_MOUSE:
            return IsMouseButtonDown(button);
    }
    return false;
}