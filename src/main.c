#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
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
#include "console.h"
#include "input.h"
#include "sprite.h"
#include "world.h"
#include "mole.h"

Mole mole;
World world;

#include "console.c"
#include "input.c"
#include "sprite.c"
#include "world.c"
#include "mole.c"


//------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------
//------------------------------------------------------------------------------
Sprite lose;

void UpdateDrawFrame()
{
    // handle input
    Vector2 movement = input_get_dir();
 
    if (input_is_button_pressed(0))
        mole.stoneEaterBonus=600;

    if (input_is_button_pressed(1))
        mole.speedBonus=300;

    if (input_is_button_pressed(2)) 
        mole_explode(&mole);

    if(mole.health>0){
        // update
        world_update(&world, &mole);
        mole_update(&mole, &movement, world.current_bitmap);
    }
    // draw
    BeginDrawing();
        ClearBackground(BLACK);
        world_draw(&world);
        mole_draw(&mole);
        char text[256] = {0};
        snprintf(text, 256, "depth: %2.2d\npoints: %d\ninput: %s\nfullscreen: F\nexit: ESC", world.depth, mole.points, input_get_device_name());
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
    InitAudioDevice();
    SetTargetFPS(FPS);

    /***************************************************************************
     * Init stuff
     ****************************************************************************/
    mole_init(&mole, 30, 30);
    world_init(&world);
    sprite_init(&lose, "resources/lose.png",1, WIDTH/2, HEIGHT/2, 15, 0);

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
    FreeConsole();

    return 0;
}
