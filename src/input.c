#include "input.h"

Vector2 input_get_dir() {
    Vector2 dir = { 0, 0 };

    // Keyboard
    if (IsKeyDown(KEY_RIGHT)) dir.x = 1;
    if (IsKeyDown(KEY_LEFT)) dir.x = -1;
    if (IsKeyDown(KEY_UP)) dir.y = -1;
    if (IsKeyDown(KEY_DOWN)) dir.y = 1;

    return Vector2Normalize(dir);
}

bool input_is_button_pressed(int button) {
    switch(button) {
        case 0: return IsKeyDown(KBD_BUTTON_0);
        case 1: return IsKeyDown(KBD_BUTTON_1);
    }
    return false;
}