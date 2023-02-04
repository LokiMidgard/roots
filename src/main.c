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
const int screenWidth = 960;
const int screenHeight = 540;
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

    Color brush[10*10] = {0};
    for (int index = 0; index < 10*10; ++index)
    {
        brush[index] = BLACK;
    }

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        BeginDrawing();
        //ClearBackground(RAYWHITE);
        int x = GetMouseX();
        int y = GetMouseY();
        Rectangle target = {x, y, 10.0f, 10.0f};
        UpdateTextureRec(screen_texture, target, brush);
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

