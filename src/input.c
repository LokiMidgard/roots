#include "config.h"
#include "input.h"

int g_device = INPUT_KEYBOARD;
Sprite* g_sprite = (Sprite*)0;
int g_mouse_deadzone = 20;
int g_gamepad = 0;
float g_gamepad_deadzone = 0.2f;

int oldWidth = 0;
int oldHeight = 0;

bool input_set_device(int device) {
    switch(device) {
        case INPUT_GAMEPAD:
            g_device = device;
            g_gamepad = 0;
        default:
            g_device = device;
            break;
    }
    return true;
}

void input_set_mouse_center(Sprite* sprite) { g_sprite = sprite; }

void input_set_mouse_deadzone(int deadzone) { g_mouse_deadzone = deadzone; }

int input_get_device() {
    return g_device == INPUT_GAMEPAD && !IsGamepadAvailable(g_gamepad) ? INPUT_KEYBOARD : g_device;
}

Vector2 input_get_dir() {
    Vector2 dir = { 0, 0 };

    if (IsKeyPressed(KEY_F)) {
        if (!IsWindowFullscreen()) {
            oldWidth = GetScreenWidth();
            oldHeight = GetScreenHeight();
            int mon = GetCurrentMonitor();
            SetWindowSize(GetMonitorWidth(mon), GetMonitorHeight(mon));
            ToggleFullscreen();
        } else {
            ToggleFullscreen();
            SetWindowSize(oldWidth, oldHeight);
        }
    }

    int device = input_get_device();

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
            dir.x -= g_sprite ? g_sprite->position.x : (WIDTH >> 1);
            dir.y -= g_sprite ? g_sprite->position.y : (HEIGHT >> 1);

            if (abs(dir.x) < g_mouse_deadzone) dir.x = 0;
            if (abs(dir.y) < g_mouse_deadzone) dir.y = 0;
            break;

        case INPUT_GAMEPAD:
            dir.x = GetGamepadAxisMovement(g_gamepad, GAMEPAD_AXIS_LEFT_X);
            dir.y = GetGamepadAxisMovement(g_gamepad, GAMEPAD_AXIS_LEFT_Y);

            if (fabsf(dir.x) < g_gamepad_deadzone) dir.x = 0.0f;
            if (fabsf(dir.y) < g_gamepad_deadzone) dir.y = 0.0f;
            break;
    }

    return Vector2Normalize(dir);
}

bool input_is_button_pressed(int button) {
    switch(input_get_device()) {
        case INPUT_KEYBOARD:
            switch(button) {
                case 0: return IsKeyDown(KBD_BUTTON_0);
                case 1: return IsKeyDown(KBD_BUTTON_1);
                case 2: return IsKeyDown(KBD_BUTTON_2);
            }
            break;
        case INPUT_MOUSE:
            return IsMouseButtonDown(button);
        case INPUT_GAMEPAD:
            switch(button) {
                case 0: return IsGamepadButtonDown(g_gamepad, GPD_BUTTON_0);
                case 1: return IsGamepadButtonDown(g_gamepad, GPD_BUTTON_1);
                case 2: return IsGamepadButtonDown(g_gamepad, GPD_BUTTON_2);
            }
    }
    return false;
}

const char* input_get_device_name() {
    switch(input_get_device()) {
        case INPUT_GAMEPAD: return "Gamepad";
        case INPUT_MOUSE: return "Mouse";
        case INPUT_KEYBOARD: return "Keyboard";
    }
    return "Unknown";
}
