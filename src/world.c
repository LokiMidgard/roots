#include <stdio.h>
#include "raylib.h"
#include "config.h"
#include "world.h"
#include "mole.h"

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

void world_init(World *world)
{
    world->speed = 0.3f;
    world->pos_remainder = 100;
    int index = 0;
    index = world->number_of_images++;
    world->images[index] = LoadImage("resources/tile_1.png");
    index = world->number_of_images++;
    world->images[index] = LoadImage("resources/tile_2.png");
    index = world->number_of_images++;
    world->images[index] = LoadImage("resources/tile_3.png");
    index = world->number_of_images++;
    world->images[index] = LoadImage("resources/tile_4.png");
    index = world->number_of_images++;
    world->images[index] = LoadImage("resources/tile_5.png");
    world->current_bitmap = LoadImageColors(world->images[0]);
    world->next_bitmap = LoadImageColors(world->images[1]);
    world->screen_texture = LoadTextureFromImage(world->images[0]);

    world->leftSpeed = 20;
    world->rightSpeed = 20;
    world->centerSpeed = 20;

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

Dig world_dig(World *world, int x, int y, int radius)
{
    Dig dig = {0};
    for (int offsetX = -radius; offsetX < radius; ++offsetX)
    {
        for (int offsetY = -radius; offsetY < radius; ++offsetY)
        {
            if (sqrt(offsetX * offsetX + offsetY * offsetY) <= radius)
            {
                Color *c = world_get_terrain(world, x + offsetX, y + offsetY);
                for(int t = EARTH; t < TerrainTypeSize; ++t) {
                    if (t == TUNNEL) continue;
                    if (color_are_equal(*c, terrain_type_to_color(t)))
                        dig.types[t] += 1;
                }
                world_set_terrain(world, x + offsetX, y + offsetY, TERRA_TUNEL);
            }
        }
    }
    return (dig);
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
            int selected_image = rand() % world->number_of_images;
            world->next_bitmap = LoadImageColors(world->images[selected_image]);
        }
    }
}
void world_update(World *world, Mole *mole)
{

    int alternate = -1;

    world->pos_remainder += world->speed;
    world_scroll(world, &mole->sprite);

    if (rand() % 1000 < 1)
    {
        world->leftSpeed = rand() % (HEIGHT - 100) + 50;
        world->rightSpeed = rand() % (HEIGHT - 100) + 50;
        world->centerSpeed = rand() % (HEIGHT - 100) + 50;

        printf("left: %d\ncenter: %d\nright: %d\n", world->leftSpeed, world->centerSpeed, world->rightSpeed);
    }

    // update pixles
    for (int x = 0; x < WIDTH; x++)
        for (int y = HEIGHT - 1; y > 0; y--)
        {
            alternate *= -1;

            Color *current = world_get_terrain(world, x, y);
            if (IS_COLOR(current, TERRA_ROOT_TIP))
            {
                int targetHeight = x < WIDTH / 3
                                       ? world->leftSpeed
                                   : x < 2 * WIDTH / 3
                                       ? world->centerSpeed
                                       : world->leftSpeed;

                // if (rand() % targetHeight < y)
                //     continue;
                if (rand() % 100 > 20)
                    continue;

                unsigned char direction = current->a & 7;
                unsigned char age = current->a > 3;

                if (age > 5 + (rand() % 5))
                {
                    age = 0;
                    direction = rand() % 7;
                }
                age++;

                direction = 0;

                if (rand() % 1000 < 5)
                {
                    world_set_terrain(world, x, y, TERRA_ROOT_KNOT);
                }
                else
                {
                    world_set_terrain(world, x, y, TERRA_ROOT);
                }

                int r = rand() % 1000;
                r -= 300;
                r += 600 * (float)direction / 7;

                Color new_tip = TERRA_ROOT_TIP;
                new_tip.a = age < 3 | direction;

                if (r > 333 && r < 666)
                {
                    world_set_terrain(world, x, y + 1, new_tip);
                }
                else if ((r < 666 && x > 0) || x >= WIDTH - 1)
                {
                    world_set_terrain(world, x - 1 * alternate, y + 1, new_tip);
                }
                else
                {
                    world_set_terrain(world, x + 1 * alternate, y + 1, new_tip);
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

                if (rand() % 100 < 40)
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
