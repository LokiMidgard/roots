#include <stdio.h>
#include "raylib.h"
#include "config.h"
#include "world.h"

void world_init(World* world) {
    world->speed = 0.3f;
    world->pos_remainder = 100;
    world->image = GenImageColor(WIDTH, HEIGHT, TERRA_EARTH);
    world->bitmap = LoadImageColors(world->image);
    world->screen_texture = LoadTextureFromImage(world->image);

    for (int i = 0; i < NUM_SEEDS; ++i)
    {
        int x = rand() % WIDTH;
        int y = HEIGHT - 1;
        for (int offset = -3; offset < 3; ++offset)
        {
            world->bitmap[POS(x + offset, y)] = TERRA_STONE;
        }
    }
}

void world_scroll(World *world, Sprite *mole)
{
    Color* bitmap = world->bitmap;
    int num_lines_to_scroll = (int)world->pos_remainder;
    mole->position.y -= num_lines_to_scroll;
    for (int i = 0; i < num_lines_to_scroll; ++i)
    {
        world->depth += 1;
        // move world 1 pixel up
        char *dst = (char *)bitmap;
        char *src = (char *)(bitmap + WIDTH);
        size_t num_bytes = sizeof(Color) * WIDTH * (HEIGHT - 1);
        memmove(dst, src, num_bytes);

        // draw new bottom line
        int y = HEIGHT - 1;
        for (int x = 1; x < WIDTH - 1; ++x)
        {
            Color *current = &bitmap[POS(x, y)];
            Color *right = &bitmap[POS(x + 1, y)];
            Color *left = &bitmap[POS(x - 1, y)];
            Color *above = &bitmap[POS(x, y - 1)];

            if (IS_COLOR(current, TERRA_STONE))
            {
                if (rand() % 15 < 8)
                {
                    bitmap[POS(x, y)] = TERRA_STONE;
                }
                else
                {
                    if (IS_COLOR(above, TERRA_STONE) && !IS_COLOR(left, TERRA_STONE))
                    {
                        int offset = rand() % 3 - 1;
                        bitmap[POS(x + offset, y)] = TERRA_STONE;
                    }
                    else
                    {
                        bitmap[POS(x, y)] = TERRA_EARTH;
                    }
                }
            }
            if (IS_COLOR(right, TERRA_STONE))
            {
                if (rand() % 15 < 8)
                    bitmap[POS(x, y)] = TERRA_STONE;
            }
        }
    }
    world->pos_remainder -= num_lines_to_scroll;
}

void world_update(World *world, Sprite *mole)
{
    world->pos_remainder += world->speed;
    world_scroll(world, mole);
}

void world_draw(World* world) {
    Rectangle srcRect = {0, 0, WIDTH, HEIGHT};
    Rectangle dstRect = {0, 0, GetScreenWidth(), GetScreenHeight()};
    Vector2 origin = {0, 0};
    UpdateTexture(world->screen_texture, world->bitmap);
    DrawTexturePro(world->screen_texture, srcRect, dstRect, origin, 0.0f, WHITE);
}
