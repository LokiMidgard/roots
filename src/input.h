#ifndef ROOTS_INPUT_H
#define ROOTS_INPUT_H

#include "raylib.h"
#include "raymath.h"

#define KBD_BUTTON_0 KEY_SPACE
#define KBD_BUTTON_1 KEY_COMMA

Vector2 input_get_dir();
bool input_is_button_pressed(int button);

#endif