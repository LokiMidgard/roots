#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include "raylib.h"
#include "raymath.h"

#include "config.h"
#include "console.c"
#include "sprite.c"
#include "world.c"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------
Sprite mole;
World world;


void mole_update(Sprite *mole, Vector2 *movement, Color *bitmap)
{
    // dig
    int mole_width = (mole->image.width / mole->number_of_frames);
    int mole_height = mole->image.height;
    Vector2 new_mole_position = Vector2Add(mole->position, *movement);
    bool collide = false;
    for (int offsetX = -mole_width / 2; offsetX < mole_width / 2; ++offsetX)
        for (int offsetY = -mole_height / 2; offsetY < mole_height / 2; ++offsetY)
        {
            if ((offsetY != -mole_height / 2 && offsetY != mole_height / 2 - 1) || (offsetX != -mole_width / 2 && offsetX != mole_width / 2 - 1))
            {
                Color c = bitmap[POS((int)new_mole_position.x + offsetX, (int)new_mole_position.y + offsetY)];
                if (c.r == TERRA_STONE.r)
                {
                    collide = true;
                }
            }
        }
    if (collide)
    {
        *movement = Vector2Zero();
    }
    for (int offsetX = -mole_width / 2; offsetX < mole_width / 2; ++offsetX)
        for (int offsetY = -mole_height / 2; offsetY < mole_height / 2; ++offsetY)
        {
            if ((offsetY != -mole_height / 2 && offsetY != mole_height / 2 - 1) || (offsetX != -mole_width / 2 && offsetX != mole_width / 2 - 1))
                bitmap[POS((int)mole->position.x + offsetX, (int)mole->position.y + offsetY)] = TERRA_TUNEL;
        }

    sprite_update(mole, movement);

    // ensure position is in bounds
    if (mole->position.x < mole->image.width / mole->number_of_frames)
    {
        mole->position.x = mole->image.width / mole->number_of_frames;
    }
    if (mole->position.y < mole->image.height)
    {
        mole->position.y = mole->image.height;
    }

    if (mole->position.x > WIDTH - mole->image.width / mole->number_of_frames)
    {
        mole->position.x = WIDTH - mole->image.width / mole->number_of_frames;
    }
    if (mole->position.y > HEIGHT - mole->image.height)
    {
        mole->position.y = HEIGHT - mole->image.height;
    }
}

void UpdateDrawFrame()
{
        // controles
        Vector2 movement = {0, 0};

        if (IsKeyDown(KEY_RIGHT))
            movement.x = movement.x + 1;
        if (IsKeyDown(KEY_LEFT))
            movement.x = movement.x - 1;
        if (IsKeyDown(KEY_UP))
            movement.y = movement.y - 1;
        if (IsKeyDown(KEY_DOWN))
            movement.y = movement.y + 1;

        movement = Vector2Normalize(movement);

        world_update(&world, &mole);
        mole_update(&mole, &movement, world.bitmap);

        BeginDrawing();

        world_draw(&world);
        sprite_draw(&mole);

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
    SetTargetFPS(FPS);

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, FPS, 1);
#else

    /***************************************************************************
     * Init stuff
     ****************************************************************************/
    sprite_init(&mole, "resources/mole.png", 8, 30, 30, 15, 0);
    world_init(&world);

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
