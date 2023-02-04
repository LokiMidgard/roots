#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include "raylib.h"

#include "config.h"
#include "console.h"
#include "input.h"
#include "sprite.h"
#include "world.h"
#include "mole.h"

#include "console.c"
#include "input.c"
#include "sprite.c"
#include "world.c"
#include "mole.c"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

//------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//------------------------------------------------------------------------------
Mole mole;
World world;
Sprite lose;

void UpdateDrawFrame()
{
    // handle input
    Vector2 movement = input_get_dir();
 
    if (input_is_button_pressed(0))
        mole.stoneEaterBonus=600;

    if (input_is_button_pressed(1))
        mole.speedBonus=300;

    if(mole.health>0){
        // update
        world_update(&world, &mole);
        mole_update(&mole, &movement, world.bitmap);
    }
    // draw
    BeginDrawing();

        world_draw(&world);
        mole_draw(&mole);
        char text[256] = {0};
        snprintf(text, 256, "depth: %2.2d\npoints: %d\ninput: %s", world.depth, mole.points, input_get_device_name());
        DrawText(text, 10, 10, 14, WHITE);
        
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
    InitConsole();
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "raylib");
    // InitAudioDevice();
    SetTargetFPS(FPS);

    /***************************************************************************
     * Init stuff
     ****************************************************************************/
    mole_init(&mole, 30, 30);
    world_init(&world);
    sprite_init(&lose, "resources/lose.png",1, WIDTH/2, HEIGHT/2, 15, 0);

    input_set_mouse_center(&mole.sprite);
    if (!input_set_device(INPUT_GAMEPAD)) {
        // input_set_device(INPUT_MOUSE);
    }

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
    FreeConsole();

    return 0;
}
