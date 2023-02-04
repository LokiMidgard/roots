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

#define TERRA_EARTH (BROWN)
#define TERRA_TUNEL (DARKBROWN)
#define TERRA_STONE (GRAY)
#define TERRA_ROOT (BLACK)

#define NUM_SEEDS (30)

float world_spd = 0.3f;
float world_pos_remainder = 0.0f;

void world_scroll(Color *world)
{
    int num_lines_to_scroll = (int)world_pos_remainder;
    for(int i=0; i<num_lines_to_scroll; ++i)
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
            Color *right    = &world[POS(x + 1, y)];
            Color *left    = &world[POS(x-1,y)];
            Color *above   = &world[POS(x, y-1)];

            if (IS_COLOR(current, TERRA_STONE))
            {
                if (rand() % 15 < 8) {
                    world[POS(x, y)] = TERRA_STONE;
                } else {
                    if (IS_COLOR(above, TERRA_STONE) && !IS_COLOR(left, TERRA_STONE)) {
                        int offset = rand() % 3 - 1;
                        world[POS(x + offset, y)] = TERRA_STONE;
                    } else {
                        world[POS(x, y)] = TERRA_EARTH;
                    }
                }
            }
            if (IS_COLOR(right, TERRA_STONE))
            {
                if (rand() % 15 < 8)
                    world[POS(x, y)] = TERRA_STONE;
            }
        }
    }
    world_pos_remainder -= num_lines_to_scroll;
}

void world_update(Color* world)
{
    world_pos_remainder += world_spd;
    world_scroll(world);
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
     * Create inital world
     ****************************************************************************/
    Image world_image = GenImageColor(WIDTH, HEIGHT, TERRA_EARTH);
    Texture2D screen_texture = LoadTextureFromImage(world_image);
    Color *world = LoadImageColors(world_image);

    // draw initial bottom line
    for (int i = 0; i < NUM_SEEDS; ++i)
    {
        int x = rand() % WIDTH;
        int y = HEIGHT - 1;
        for (int offset = -3; offset < 3; ++offset)
        {
            world[POS(x + offset, y)] = TERRA_STONE;
        }
    }

    // pre-scroll some lines
    world_pos_remainder = 100;
    world_scroll(world);

    /***************************************************************************
     * Create character
     ****************************************************************************/
    Sprite mole;
    sprite_init(&mole, "resources/mole.png", 8, 30, 30, 15, 0);

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

        world_update(world);
        sprite_update(&mole, &movement);

        // dig
        for (int offsetX = -(mole.image.width/mole.number_of_frames) / 2; offsetX < (mole.image.width/mole.number_of_frames) / 2; ++offsetX)
            for (int offsetY = -mole.image.height / 2; offsetY < mole.image.height / 2; ++offsetY)
            {
                world[POS((int)mole.position.x + offsetX, (int)mole.position.y + offsetY)] = TERRA_TUNEL;
            }

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
