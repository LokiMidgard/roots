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