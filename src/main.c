#include <stdio.h>
#include <conio.h>
#include "raylib.h"

int __stdcall AllocConsole(void);
void __stdcall FreeConsole(void);

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------
const int screenWidth = 960;
const int screenHeight = 540;

//----------------------------------------------------------------------------------
// Local Functions Declaration
//----------------------------------------------------------------------------------
static void UpdateDrawFrame(void);          // Update and draw one frame

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
int main() 
{
    FILE *ConsoleIn  = 0;
    FILE *ConsoleOut = 0;
    FILE *ConsoleErr = 0;
    AllocConsole();
    freopen_s(&ConsoleIn, "CONIN$", "r", stdin);
    freopen_s(&ConsoleOut, "CONOUT$", "w", stdout);
    freopen_s(&ConsoleErr, "CONOUT$", "w", stderr);


    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(screenWidth, screenHeight, "raylib");

    //--------------------------------------------------------------------------------------

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();                  // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    getch();
    FreeConsole();

    return 0;
}

float rotation = 0.0f;
// Update and draw game frame
static void UpdateDrawFrame(void)
{

    // Update
    //----------------------------------------------------------------------------------
    rotation += 0.2f;
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawText("some basic shapes available on raylib", 20, 20, 20, DARKGRAY);

    // Circle shapes and lines
    DrawCircle(screenWidth/5, 120, 35, DARKBLUE);
    DrawCircleGradient(screenWidth/5, 220, 60, GREEN, SKYBLUE);
    DrawCircleLines(screenWidth/5, 340, 80, DARKBLUE);

    // Rectangle shapes and lines
    DrawRectangle(screenWidth/4*2 - 60, 100, 120, 60, RED);
    DrawRectangleGradientH(screenWidth/4*2 - 90, 170, 180, 130, MAROON, GOLD);
    DrawRectangleLines(screenWidth/4*2 - 40, 320, 80, 60, ORANGE);  // NOTE: Uses QUADS internally, not lines

    // Triangle shapes and lines
    DrawTriangle((Vector2){ screenWidth/4.0f *3.0f, 80.0f },
                 (Vector2){ screenWidth/4.0f *3.0f - 60.0f, 150.0f },
                 (Vector2){ screenWidth/4.0f *3.0f + 60.0f, 150.0f }, VIOLET);

    DrawTriangleLines((Vector2){ screenWidth/4.0f*3.0f, 160.0f },
                      (Vector2){ screenWidth/4.0f*3.0f - 20.0f, 230.0f },
                      (Vector2){ screenWidth/4.0f*3.0f + 20.0f, 230.0f }, DARKBLUE);

    // Polygon shapes and lines
    DrawPoly((Vector2){ screenWidth/4.0f*3, 330 }, 6, 80, rotation, BROWN);
    DrawPolyLines((Vector2){ screenWidth/4.0f*3, 330 }, 6, 90, rotation, BROWN);
    DrawPolyLinesEx((Vector2){ screenWidth/4.0f*3, 330 }, 6, 85, rotation, 6, BEIGE);

    // NOTE: We draw all LINES based shapes together to optimize internal drawing,
    // this way, all LINES are rendered in a single draw pass
    DrawLine(18, 42, screenWidth - 18, 42, BLACK);

    EndDrawing();
    //----------------------------------------------------------------------------------
}
