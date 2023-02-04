#include <stdio.h>
#include "raylib.h"
#include "config.h"
#include "world.h"
#include "mole.h"

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

    for (int i = 0; i < NUM_SEEDS; ++i)
    {
        // int x = rand() % WIDTH;
        // int y = HEIGHT - 1;
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
            int selected_image = rand()%world->number_of_images;
            printf("selected: %d\n", selected_image);
            world->next_bitmap = LoadImageColors(world->images[selected_image]);
        }
    }
}

void world_update(World *world, Mole *mole)
{
    world->pos_remainder += world->speed;
    world_scroll(world, &mole->sprite);
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
