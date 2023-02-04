#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include "raylib.h"

#include "config.h"
#include "sprite.h"
#include "world.h"
#include "mole.h"

#include "console.c"
#include "sprite.c"
#include "world.c"
#include "mole.c"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------
Mole mole;
World world;

void UpdateDrawFrame()
{
    // handle input
    Vector2 movement = {0, 0};

    if (IsKeyDown(KEY_RIGHT))
        movement.x = movement.x + 1;
    if (IsKeyDown(KEY_LEFT))
        movement.x = movement.x - 1;
    if (IsKeyDown(KEY_UP))
        movement.y = movement.y - 1;
    if (IsKeyDown(KEY_DOWN))
        movement.y = movement.y + 1;

    

    // update
    world_update(&world, &mole);
    mole_update(&mole, &movement, world.bitmap);

    // draw
    BeginDrawing();

        world_draw(&world);
        mole_draw(&mole);
        char text[256] = {0};
        snprintf(text, 256, "depth: %2.2d\n", world.depth);
        DrawText(text, 10, 10, 14, WHITE);


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
