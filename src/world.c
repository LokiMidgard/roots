#include <stdio.h>
#include "raylib.h"
#include "config.h"
#include "world.h"
#include "mole.h"

void world_init(World *world)
{
    world->speed = 0.3f;
    world->pos_remainder = 100;
    world->image = LoadImage("resources/tile.png");
    world->current_bitmap = LoadImageColors(world->image);
    world->next_bitmap = LoadImageColors(world->image);
    world->screen_texture = LoadTextureFromImage(world->image);

    for (int i = 0; i < NUM_SEEDS; ++i)
    {
        int x = rand() % WIDTH;
        int y = HEIGHT - 1;
        world_set_terrain(world, x, y, TERRA_ROOT_TIP);
        for (int offset = -3; offset < 3; ++offset)
        {
            // world->bitmap[POS(x + offset, y)] = TERRA_STONE;
        }
    }
}

void world_set_terrain(World *world, int x, int y, Color color)
{
    Color *bitmap = world->current_bitmap;
    int scroll_amount = world->depth;
    int from_the_top = HEIGHT - scroll_amount;
    if (y >= from_the_top)
    {
        y -= from_the_top;
        bitmap = world->next_bitmap;
    }
    else
    {
        y += world->depth;
    }
    if (x < 0 || y < 0 || x >= WIDTH || y >= HEIGHT)
        return;
    bitmap[POS(x, y)] = color;
}

Color *
world_get_terrain(World *world, int x, int y)
{
    Color *bitmap = world->current_bitmap;
    int scroll_amount = world->depth;
    int from_the_top = HEIGHT - scroll_amount;
    if (y >= from_the_top)
    {
        y -= from_the_top;
        bitmap = world->next_bitmap;
    }
    else
    {
        y += world->depth;
    }
    if (x < 0 || y < 0 || x >= WIDTH || y >= HEIGHT)
        return &TERRA_EARTH;
    return bitmap + POS(x, y);
}

int color_are_equal(Color c1, Color c2)
{
    return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b;
}

void world_scroll(World *world, Sprite *mole)
{
    for (int i = 0; i < world->pos_remainder; i++)
    {
        world->pos_remainder -= 1;
        world->depth += 1;
        mole->position.y -= 1;
        if (world->depth == HEIGHT)
        {
            world->depth = 0;
            UnloadImageColors(world->current_bitmap);
            world->current_bitmap = world->next_bitmap;
            world->next_bitmap = LoadImageColors(world->image);
        }
    }
}
int alternate = -1;
void world_update(World *world, Mole *mole)
{
    
          alternate *=-1;
    
    world->pos_remainder += world->speed;
    world_scroll(world, &mole->sprite);

    // update pixles
    for (int x = 0; x < WIDTH; x++)
        for (int y = HEIGHT - 1; y > 0; y--)
        {

            Color *current = world_get_terrain(world, x, y);
            if (IS_COLOR(current, TERRA_ROOT_TIP))
            {
                if (rand() % 100 > 20)
                    continue;

                if (rand() % 100 < 5)
                {
                    world_set_terrain(world, x, y, TERRA_ROOT_KNOT);
                }
                else
                {
                    world_set_terrain(world, x, y, TERRA_ROOT);
                }

                if (rand() % 100 < 80)
                {
                    world_set_terrain(world, x, y + 1, TERRA_ROOT_TIP);
                }
                else if ((rand() % 1 < 1 && x > 0) || x >= WIDTH - 1)
                {
                    world_set_terrain(world, x - 1 * alternate, y + 1, TERRA_ROOT_TIP);
                }
                else
                {
                    world_set_terrain(world, x + 1 * alternate, y + 1, TERRA_ROOT_TIP);
                }
            }
            else if (IS_COLOR(current, TERRA_ROOT_KNOT))
            {
                if (rand() % 100 > 5)
                    continue;

                if (rand() % 100 < 2)
                {
                    world_set_terrain(world, x, y, TERRA_ROOT_KNOT);
                }
                else
                {
                    world_set_terrain(world, x, y, TERRA_ROOT);
                }

                if (rand() % 100 < 80)
                {
                    world_set_terrain(world, x, y + 1, TERRA_ROOT_TIP);
                }
                else if ((rand() % 1 < 1 && x > 0) || x >= WIDTH - 1)
                {
                    world_set_terrain(world, x - 1 * alternate, y + 1, TERRA_ROOT_TIP);
                }
                else
                {
                    world_set_terrain(world, x + 1 * alternate, y + 1, TERRA_ROOT_TIP);
                }
            }
        }
}

void world_draw(World *world)
{
    Rectangle srcRect = {0, 0, WIDTH, HEIGHT};
    Rectangle dstRect = {0, 0, GetScreenWidth(), GetScreenHeight()};
    Vector2 origin = {0, 0};

    Rectangle upper_screen = {0, 0, WIDTH, HEIGHT - world->depth};
    Rectangle lower_screen = {0, HEIGHT - world->depth, WIDTH, world->depth};
    UpdateTextureRec(world->screen_texture, upper_screen, world->current_bitmap + (world->depth * WIDTH));
    UpdateTextureRec(world->screen_texture, lower_screen, world->next_bitmap);
    DrawTexturePro(world->screen_texture, srcRect, dstRect, origin, 0.0f, WHITE);
}
