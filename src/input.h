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
#define GPD_BUTTON_3 GAMEPAD_BUTTON_RIGHT_FACE_LEFT

typedef struct Input {
    int device;
    Sprite* sprite;
    int mouse_deadzone;
    int gamepad;
    float gamepad_deadzone;
} Input;

void input_init(Input* input);

bool input_set_device(Input* input, int device);
void input_set_mouse_center(Input* input, Sprite* sprite);

Vector2 input_get_dir(Input* input);
bool input_is_button_pressed(Input* input, int button);
bool input_is_button_down(Input* input, int button);

const char* input_get_device_name(Input* input);

#endif
