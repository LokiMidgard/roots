#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include "raylib.h"
#include "raymath.h"

#include "config.h"
#include "console.c"
#include "sprite.c"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------
#define FPS (60)
#define WIDTH (960)
#define HEIGHT (540)
#define POS(x, y) ((y) * (WIDTH) + (x))

#define NUM_SEEDS (30)


void scroll_world(Color *world)
{
    // move world 1 pixel up
    char *dst = (char *)world;
    char *src = (char *)(world + WIDTH);
    size_t num_bytes = sizeof(Color) * WIDTH * (HEIGHT - 1);
    memmove(dst, src, num_bytes);

    // draw new bottom line
    int y = HEIGHT - 1;
    for (int x = 1; x < WIDTH - 1; ++x)
    {
        Color *current = &world[POS(x, y)];
        Color *next = &world[POS(x + 1, y)];
        // Color last    = world[POS(x-1,y)];

        if (current->r == GRAY.r)
        {
            if (rand() % 15 < 8)
                world[POS(x, y)] = GRAY;
            else
                world[POS(x, y)] = BROWN;
        }
        if (next->r == GRAY.r)
        {
            if (rand() % 15 < 8)
                world[POS(x, y)] = GRAY;
        }
    }
}

int main()
{
    /***************************************************************************
     * Init technical stuff
     ****************************************************************************/
    InitConsole();
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(WIDTH, HEIGHT, "raylib");
    SetTargetFPS(FPS);

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, FPS, 1);
#else

    /***************************************************************************
     * Create inital world
     ****************************************************************************/
    Image world_image = GenImageColor(WIDTH, HEIGHT, BROWN);
    Texture2D screen_texture = LoadTextureFromImage(world_image);
    Color *world = LoadImageColors(world_image);

    // draw initial bottom line
    for (int i = 0; i < NUM_SEEDS; ++i)
    {
        int x = rand() % WIDTH;
        int y = HEIGHT - 1;
        for (int offset = -3; offset < 3; ++offset)
        {
            world[POS(x + offset, y)] = GRAY;
        }
    }

    // pre-scroll some lines
    for (int i = 0; i < 10; ++i)
    {
        scroll_world(world);
    }

    /***************************************************************************
     * Create character
     ****************************************************************************/
    Sprite mole;
    mole.counter = 0;
    mole.image = LoadTexture("resources/mole.png");
    mole.number_of_frames = 8;
    mole.position.x = 30;
    mole.position.y = 30;
    mole.speed = 15;

    /***************************************************************************
     * Main Loop
     ****************************************************************************/
    while (!WindowShouldClose()) // Detect window close button or ESC key
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

        scroll_world(world);
        sprite_update(&mole, &movement);

        BeginDrawing();
        // ClearBackground(RAYWHITE);

        UpdateTexture(screen_texture, world);

        // texture to screen with scaling
        Rectangle srcRect = {0, 0, WIDTH, HEIGHT};
        Rectangle dstRect = {0, 0, GetScreenWidth(), GetScreenHeight()};
        Vector2 origin = {0, 0};
        DrawTexturePro(screen_texture, srcRect, dstRect, origin, 0.0f, WHITE);

        sprite_draw(&mole);

        EndDrawing();
    }
#endif

    /***************************************************************************
     * Cleanup
     ****************************************************************************/
    CloseWindow();
    FreeConsole();

    return 0;
}
