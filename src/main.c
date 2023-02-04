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

void world_scroll(Color *world, Sprite *mole)
{
    int num_lines_to_scroll = (int)world_pos_remainder;
    mole->position.y -= num_lines_to_scroll;
    for (int i = 0; i < num_lines_to_scroll; ++i)
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
            Color *right = &world[POS(x + 1, y)];
            Color *left = &world[POS(x - 1, y)];
            Color *above = &world[POS(x, y - 1)];

            if (IS_COLOR(current, TERRA_STONE))
            {
                if (rand() % 15 < 8)
                {
                    world[POS(x, y)] = TERRA_STONE;
                }
                else
                {
                    if (IS_COLOR(above, TERRA_STONE) && !IS_COLOR(left, TERRA_STONE))
                    {
                        int offset = rand() % 3 - 1;
                        world[POS(x + offset, y)] = TERRA_STONE;
                    }
                    else
                    {
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

void world_update(Color *world, Sprite *mole)
{
    world_pos_remainder += world_spd;
    world_scroll(world, mole);
}

void mole_update(Sprite *mole, Vector2 *movement, Color *world)
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
                Color c = world[POS((int)new_mole_position.x + offsetX, (int)new_mole_position.y + offsetY)];
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
                world[POS((int)mole->position.x + offsetX, (int)mole->position.y + offsetY)] = TERRA_TUNEL;
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
     * Create character
     ****************************************************************************/
    Sprite mole;
    sprite_init(&mole, "resources/mole.png", 8, 30, 30, 15, 0);

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
    world_scroll(world, &mole);

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

        world_update(world, &mole);
        mole_update(&mole, &movement, world);

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
