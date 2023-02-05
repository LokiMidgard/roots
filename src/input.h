#ifndef ROOTS_INPUT_H
#define ROOTS_INPUT_H

#include "raylib.h"
#include "raymath.h"
#include "sprite.h"

#define INPUT_KEYBOARD 0
#define INPUT_MOUSE 1
#define INPUT_GAMEPAD 2

#define KBD_BUTTON_0 KEY_SPACE
#define KBD_BUTTON_1 KEY_COMMA
#define KBD_BUTTON_2 KEY_PERIOD

#define GPD_BUTTON_0 GAMEPAD_BUTTON_RIGHT_FACE_DOWN
#define GPD_BUTTON_1 GAMEPAD_BUTTON_RIGHT_FACE_RIGHT
#define GPD_BUTTON_2 GAMEPAD_BUTTON_RIGHT_FACE_UP

bool input_set_device(int device);
void input_set_mouse_center(Sprite* sprite);

Vector2 input_get_dir();
bool input_is_button_pressed(int button);
bool input_is_button_down(int button);

const char* input_get_device_name();

#endif