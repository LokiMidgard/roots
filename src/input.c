#include "config.h"
#include "input.h"

int g_device = INPUT_KEYBOARD;

void input_set_device(int device) {
    g_device = device;
}

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
            dir.x -= (WIDTH >> 1);
            dir.y -= (HEIGHT >> 1);
            if (abs(dir.x) < (WIDTH / 10)) dir.x = 0;
            if (abs(dir.y) < (HEIGHT / 10)) dir.y = 0;
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