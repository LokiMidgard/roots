#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raylib.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#include "config.h"
#include "utils.h"
#include "console.h"
#include "input.h"
#include "sprite.h"
#include "world.h"
#include "mole.h"
#include "worms.h"
#include "stuff.h"
#include "particles.h"
#include "hud.h"
#include "inventory.h"

Mole mole;
World world;
Input input;
Stuff stuff;
Sprite lose;
Sprite continueS;
Hud hud;
Music music;
Font fontBm;
Sound intro;
Inventory inventory;
int endScreenTimer;

bool started = false;

#include "console.c"
#include "input.c"
#include "sprite.c"
#include "world.c"
#include "mole.c"
#include "worms.c"
#include "stuff.c"
#include "particles.c"
#include "hud.c"
#include "inventory.c"

void reset_game()
{
    mole_reset(&mole, WIDTH / 2, HEIGHT + 60);
    world_reset(&world);
    endScreenTimer = 0;
}

void init_game()
{
    world_init(&world);
    hud_init(&hud, &inventory);
    mole_init(&mole, WIDTH / 2, HEIGHT + 60);
    stuff_init(&stuff);
    inventory_init(&inventory);
}

void MainLoop()
{
    UpdateMusicStream(music);

    Vector2 movement = input_get_dir(&input);

    if (started)
    {
        if (input_is_button_pressed(&input, 0))
        {
            if (inventory_use(&inventory, st_MEAT))
            {
                mole.stoneEaterBonus = 600;
            }
        }

        if (input_is_button_pressed(&input, 1))
        {
            if (inventory_use(&inventory, st_STAR))
            {
                mole.speedBonus = 300;
            }
        }

        if (input_is_button_pressed(&input, 2))
        {
            if (inventory_use(&inventory, st_BOMB))
            {
                mole_explode(&mole);
            }
        }
    }

    if (started && mole.health > 0)
    {
        // update
        world_update(&world);
        mole.sprite.position.y -= world.last_scroll;
        mole_update(&mole, movement);

        snprintf(hud.debug_text, 256, "depth: %2.2d\npoints: %d\nhealth: %.f\ninput: %s\nfullscreen: F\nexit: ESC", world.depth, (int)mole.points, mole.health, input_get_device_name(&input));
        hud_update(&hud);
        stuff_update(&stuff, world.last_scroll);
    }

    if (!started)
    {
        if (input_any(&input))
        {
            started = true;
        }
    }

    // check for collisions
    float pickup_radius = 10.0f;
    StuffType picked_up_type;
    int success = stuff_pickup(&stuff,
                               mole.sprite.position,
                               pickup_radius,
                               &picked_up_type);
    if (success)
        inventory_add(&inventory, picked_up_type, 1);

    // draw
    BeginDrawing();
    ClearBackground(MAGENTA);
    world_draw(&world);
    stuff_draw(&stuff);
    mole_draw(&mole);
    hud_draw(&hud, &stuff);

    if (mole.health <= 0)
    {
        endScreenTimer++;
        if (endScreenTimer > 150)
        {
            sprite_draw(&continueS);
        }
        if (input_any(&input) && endScreenTimer > 150)
        {
            reset_game();
        }
        sprite_draw(&lose);
        char points[10];
        snprintf(points, 10, "%d", (int)mole.points);
        Vector2 measurment = MeasureTextEx(fontBm, points, (float)fontBm.baseSize, 2);
        DrawTextEx(fontBm, points, (Vector2){(WIDTH / 2 - measurment.x / 2) * GetScreenWidth() / WIDTH, (HEIGHT / 2) * GetScreenHeight() / HEIGHT}, (float)fontBm.baseSize, 2, WHITE);
    }

    EndDrawing();
}


int main()
{
    /***************************************************************************
     * Init technical stuff
     ****************************************************************************/
    InitConsole();
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "raylib");
    InitAudioDevice();
    SetTargetFPS(FPS);

    /***************************************************************************
     * Init stuff
     ****************************************************************************/
    Vector2 origin = {0.5, 0.5};
    sprite_init_static_with_origin(&lose, "resources/lose.png", WIDTH / 2, HEIGHT / 2 - 80, origin);
    sprite_init_static_with_origin(&continueS, "resources/continue.png", WIDTH / 2, HEIGHT / 2 - 80, origin);
    input_init(&input);

    init_game();

    input_set_mouse_center(&input, &mole.sprite);
    input_set_device(&input, INPUT_GAMEPAD);

    intro = LoadSoundEx("resources/speech/roots.wav", 2.0f);
    PlaySound(intro);

    music = LoadMusicStream("resources/bgm/doom.ogg");
    SetMusicVolume(music, 0.3f);
    PlayMusicStream(music);

    fontBm = LoadFont("resources/fonts/eater/eater-number.fnt");

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(MainLoop, FPS, 1);
#else
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        MainLoop();
    }
#endif

    /***************************************************************************
     * Cleanup
     ****************************************************************************/
    CloseWindow();
    CloseConsole();

    return 0;
}
