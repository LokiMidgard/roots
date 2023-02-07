#include "config.h"
#include "input.h"

void input_init(Input* input) {
    input->device = INPUT_KEYBOARD;
    input->sprite = (Sprite*)0;
    input->mouse_deadzone = 20;
    input->gamepad = 0;
    input->gamepad_deadzone = 0.2f;
}

bool input_set_device(Input* input, int device) {
    switch(device) {
        case INPUT_GAMEPAD:
            input->device = device;
            input->gamepad = 0;
        default:
            input->device = device;
            break;
    }
    return true;
}

void input_set_mouse_center(Input* input, Sprite* sprite) {
    input->sprite = sprite;
}

void input_set_mouse_deadzone(Input* input, int deadzone) {
    input->mouse_deadzone = deadzone;
}

int input_get_device(Input* input) {
    return input->device == INPUT_GAMEPAD && !IsGamepadAvailable(input->gamepad) ? INPUT_KEYBOARD : input->device;
}

bool input_any(Input *input)
{
    Vector2 dir = input_get_dir(input);
    return (dir.x != 0 || dir.y != 0 || input_is_button_pressed(input, 0) || input_is_button_pressed(input, 1) || input_is_button_pressed(input, 2));
}

Vector2 input_get_dir(Input* input) {
    Vector2 dir = { 0, 0 };
    int device = input_get_device(input);

    switch(device) {
        case INPUT_KEYBOARD:
            if (IsKeyDown(KEY_RIGHT)) dir.x = 1.0f;
            if (IsKeyDown(KEY_LEFT)) dir.x = -1.0f;
            if (IsKeyDown(KEY_UP)) dir.y = -1.0f;
            if (IsKeyDown(KEY_DOWN)) dir.y = 1.0f;
            break;
    
        case INPUT_MOUSE:
            dir = GetMousePosition();
            dir.x *= WIDTH/(float)GetScreenWidth();
            dir.y *= HEIGHT/(float)GetScreenHeight();
            dir.x -= input->sprite ? input->sprite->position.x : (WIDTH >> 1);
            dir.y -= input->sprite ? input->sprite->position.y : (HEIGHT >> 1);

            if (fabsf(dir.x) < input->mouse_deadzone) dir.x = 0;
            if (fabsf(dir.y) < input->mouse_deadzone) dir.y = 0;
            break;

        case INPUT_GAMEPAD:
            dir.x = GetGamepadAxisMovement(input->gamepad, GAMEPAD_AXIS_LEFT_X);
            dir.y = GetGamepadAxisMovement(input->gamepad, GAMEPAD_AXIS_LEFT_Y);

            if (fabsf(dir.x) < input->gamepad_deadzone) dir.x = 0.0f;
            if (fabsf(dir.y) < input->gamepad_deadzone) dir.y = 0.0f;
            break;
    }

    return Vector2Normalize(dir);
}

bool input_is_button_down(Input* input, int button) {
    switch(input_get_device(input)) {
        case INPUT_KEYBOARD:
            switch(button) {
                case 0: return IsKeyDown(KBD_BUTTON_0);
                case 1: return IsKeyDown(KBD_BUTTON_1);
                case 2: return IsKeyDown(KBD_BUTTON_2);
                case 3: return IsKeyDown(KBD_BUTTON_3);
            }
            break;
        case INPUT_MOUSE:
            return IsMouseButtonDown(button);
        case INPUT_GAMEPAD:
            switch(button) {
                case 0: return IsGamepadButtonDown(input->gamepad, GPD_BUTTON_0);
                case 1: return IsGamepadButtonDown(input->gamepad, GPD_BUTTON_1);
                case 2: return IsGamepadButtonDown(input->gamepad, GPD_BUTTON_2);
                case 3: return IsGamepadButtonDown(input->gamepad, GPD_BUTTON_3);
            }
    }
    return false;
}

bool input_is_button_pressed(Input* input, int button) {
    switch(input_get_device(input)) {
        case INPUT_KEYBOARD:
            switch(button) {
                case 0: return IsKeyPressed(KBD_BUTTON_0);
                case 1: return IsKeyPressed(KBD_BUTTON_1);
                case 2: return IsKeyPressed(KBD_BUTTON_2);
                case 3: return IsKeyPressed(KBD_BUTTON_3);
            }
            break;
        case INPUT_MOUSE:
            return IsMouseButtonPressed(button);
        case INPUT_GAMEPAD:
            switch(button) {
                case 0: return IsGamepadButtonPressed(input->gamepad, GPD_BUTTON_0);
                case 1: return IsGamepadButtonPressed(input->gamepad, GPD_BUTTON_1);
                case 2: return IsGamepadButtonPressed(input->gamepad, GPD_BUTTON_2);
                case 3: return IsGamepadButtonPressed(input->gamepad, GPD_BUTTON_3);
            }
    }
    return false;
}

const char* input_get_device_name(Input* input) {
    switch(input_get_device(input)) {
        case INPUT_GAMEPAD: return "Gamepad";
        case INPUT_MOUSE: return "Mouse";
        case INPUT_KEYBOARD: return "Keyboard";
    }
    return "Unknown";
}
