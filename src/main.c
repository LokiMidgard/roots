#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include "raylib.h"
#include "console.c"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------
#define screenWidth 960
#define screenHeight 540
Image screen = {0};

int number_of_seeds = 0;
int root_seeds[screenWidth];
Color *world = 0;

void update_last_world_line()
{
    memmove((char *)world, (char *)(world + screenWidth), (screenWidth * (screenHeight-1))*sizeof(Color));
    for (int x = 1; x < screenWidth - 1; ++x)
    {
        int y = screenHeight - 1;
        Color current = world[y * screenWidth + x];
        Color next    = world[y * screenWidth + x + 1];
        Color last    = world[y * screenWidth + x - 1];
        if (current.r == GRAY.r)
        {
            if (rand()%15 < 8)
                world[y * screenWidth + x] = GRAY;
            else
                world[y * screenWidth + x] = BROWN;
        }
        if (next.r == GRAY.r)
        {
            if (rand()%15 < 8)
                world[y * screenWidth + x] = GRAY;
        }
    }
}

int main() 
{
    InitConsole();
    InitWindow(screenWidth, screenHeight, "raylib");

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);

    screen = GenImageColor(screenWidth, screenHeight, BROWN);
    Texture2D screen_texture = LoadTextureFromImage(screen);


    world = LoadImageColors(screen);
    number_of_seeds = 30;
    for (int index = 0; index < number_of_seeds; ++index)
    {
        int x = rand() % screenWidth;
        int y = screenHeight - 1;
        for (int offset = -3; offset < 3; ++offset)
        {
            world[x+offset + y * screenWidth] = GRAY;
        }
    }

    for (int index = 0; index < 10; ++index)
    {
        update_last_world_line();
    }

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        update_last_world_line();
        BeginDrawing();
        //ClearBackground(RAYWHITE);
        UpdateTexture(screen_texture, world);
        DrawTexture(screen_texture, 0, 0, WHITE);
        EndDrawing();
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();                  // Close window and OpenGL context
                                    //--------------------------------------------------------------------------------------
    FreeConsole();

    return 0;
}

