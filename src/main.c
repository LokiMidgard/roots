#include <stdio.h>
#include <conio.h>
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

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
int main() 
{
    // Initialization
    //--------------------------------------------------------------------------------------
    InitConsole();
    InitWindow(screenWidth, screenHeight, "raylib");
    //--------------------------------------------------------------------------------------

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    //
    screen = GenImageColor(screenWidth, screenHeight, DARKBLUE);
    Texture2D screen_texture = LoadTextureFromImage(screen);

    Color *image = LoadImageColors(screen);
    for (int y = 0; y < 540; ++y)
    for (int x = 0; x < 960; ++x)
    {
        Color color = BLACK;
        if ((y-1) % 10 == 0 || y % 10 == 0 || (y+1) % 10 == 0)
            color = BLUE;
        image[x + y * 960] = color;
    }

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        BeginDrawing();
        //ClearBackground(RAYWHITE);
        UpdateTexture(screen_texture, image);
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

