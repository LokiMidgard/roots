#include <stdio.h>
#include <stdlib.h>
#if !defined(PLATFORM_WEB)
#include <conio.h>
#endif
#include <string.h>
#include "raylib.h"
#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif
int color_are_equal(Color c1, Color c2)
{
    return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b;
}

#include "config.h"
#if !defined(PLATFORM_WEB)
#include "console.h"
#endif
#include "input.h"
#include "sprite.h"
#include "world.h"
#include "mole.h"
#include "worms.h"
#include "stuff.h"
#include "particles.h"
#include "hud.h"

Mole mole;
World world;
Stuff stuff;
Sprite lose;
Hud hud;
int inventory[st_size];

#if !defined(PLATFORM_WEB)
#include "console.c"
#endif
#include "input.c"
#include "sprite.c"
#include "world.c"
#include "mole.c"
#include "worms.c"
#include "particles.c"
#include "stuff.c"
#include "hud.c"

void UpdateDrawFrame()
{
    // handle input
    Vector2 movement = input_get_dir();

    if (input_is_button_pressed(0))
        mole.stoneEaterBonus = 600;

    if (input_is_button_pressed(1))
        mole.speedBonus = 300;

    if (input_is_button_pressed(2))
        mole_explode(&mole);

    if (mole.health > 0)
    {
        // update
        world_update(&world);
        mole.sprite.position.y -= world.last_scroll;
        mole_update(&mole, movement);

        snprintf(hud.debug_text, 256, "depth: %2.2d\npoints: %d\nhealth: %.f\ninput: %s\nfullscreen: F\nexit: ESC", world.depth, mole.points, mole.health, input_get_device_name());
        hud_update(&hud);
    }
    stuff_update(&stuff, world.last_scroll);
    // check for collisions
    float pickup_radius = 10.0f;
    StuffType picked_up_type;
    int success = stuff_pickup(&stuff,
                               mole.sprite.position,
                               pickup_radius,
                               &picked_up_type);
    if (success)
        inventory[picked_up_type] += 1;

    // draw

    BeginDrawing();
        ClearBackground(MAGENTA);
        world_draw(&world);
        stuff_draw(&stuff);
        mole_draw(&mole);
        hud_draw(&hud, &stuff);
        
     if(mole.health<=0){
        sprite_draw(&lose);
    }

    EndDrawing();
}

int main()
{
    /***************************************************************************
     * Init technical stuff
     ****************************************************************************/
#if !defined(PLATFORM_WEB)
    InitConsole();
#endif
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "raylib");
    InitAudioDevice();
    SetTargetFPS(FPS);

    /***************************************************************************
     * Init stuff
     ****************************************************************************/
    hud_init(&hud, inventory);
    mole_init(&mole, WIDTH/2, HEIGHT+60);
    world_init(&world);
    sprite_init(&lose, "resources/lose.png", 660, 1, WIDTH/2, HEIGHT/2, 15, 0);
    stuff_init(&stuff);

    input_set_mouse_center(&mole.sprite);
    input_set_device(INPUT_GAMEPAD);

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, FPS, 1);
#else
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }
#endif

    /***************************************************************************
     * Cleanup
     ****************************************************************************/
    CloseWindow();
#if !defined(PLATFORM_WEB)
    FreeConsole();
#endif

    return 0;
}
