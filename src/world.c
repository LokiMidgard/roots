#include <stdio.h>
#include "raylib.h"
#include "config.h"
#include "world.h"
#include "mole.h"

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

void
world_set_terrain(World *world, int x, int y, Color color)
{
    if (x < 0 || y < 0 || x >= WIDTH || y >= HEIGHT)
        return;
    Color* bitmap = world->bitmap;
    bitmap[POS(x, y)] = color;
}

int
color_are_equal(Color c1, Color c2)
{
    return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b;
}

void world_scroll(World *world, Sprite *mole)
{
    Color* bitmap = world->bitmap;
    int num_lines_to_scroll = (int)world->pos_remainder;
    mole->position.y -= num_lines_to_scroll;
    for (int i = 0; i < num_lines_to_scroll; ++i)
    {
        // move world 1 pixel up
        world->depth += 1;
        char *dst = (char *)bitmap;
        char *src = (char *)(bitmap + WIDTH);
        size_t num_bytes = sizeof(Color) * WIDTH * (HEIGHT - 1);
        memmove(dst, src, num_bytes);

        // draw new bottom line
        int y = HEIGHT - 1;
        for (int x = 1; x < WIDTH; ++x)
        {
            Color *current = &bitmap[POS(x, y)];

            if (IS_COLOR(current, TERRA_EARTH))
            {
                int new_seed = rand()%100 > 98;
                if (new_seed)
                    world_set_terrain(world, x, y, TERRA_STONE);
            }
            else if (IS_COLOR(current, TERRA_STONE))
            {
                int length = 1;
                for (; length + x < WIDTH; ++length)
                {
                    Color next_color = bitmap[POS(x + length, y)];
                    if (!color_are_equal(next_color, TERRA_STONE))
                        break;

                }
                int should_grow   = rand()%100 > 50;
                int should_shrink = rand()%100 > 70;
                if (should_shrink)
                {
                    world_set_terrain(world, x, y, TERRA_EARTH);
                    world_set_terrain(world, x+length-1, y, TERRA_EARTH);
                }
                else if (should_grow)
                {
                    world_set_terrain(world, x-1, y, TERRA_STONE);
                    world_set_terrain(world, x+length, y, TERRA_STONE);
                }
            }
        }
    }
    world->pos_remainder -= num_lines_to_scroll;
}

void world_update(World *world, Mole *mole)
{
    world->pos_remainder += world->speed;
    world_scroll(world, &mole->sprite);
}

void world_draw(World* world) {
    Rectangle srcRect = {0, 0, WIDTH, HEIGHT};
    Rectangle dstRect = {0, 0, GetScreenWidth(), GetScreenHeight()};
    Vector2 origin = {0, 0};
    UpdateTexture(world->screen_texture, world->bitmap);
    DrawTexturePro(world->screen_texture, srcRect, dstRect, origin, 0.0f, WHITE);
}
