#include <stdio.h>
#include "raylib.h"
#include "config.h"
#include "world.h"
#include "mole.h"
#include "utils.h"

#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))

void world_set_terrain(World *world, int x, int y, Color color)
{
    Color *bitmap = world->current_bitmap;
    int scroll_amount = world->current_scroll;
    int from_the_top = HEIGHT - scroll_amount;
    if (y >= from_the_top)
    {
        y -= from_the_top;
        bitmap = world->next_bitmap;
    }
    else
    {
        y += world->current_scroll;
    }
    if (x < 0 || y < 0 || x >= WIDTH || y >= HEIGHT)
        return;
    bitmap[POS(x, y)] = color;
}

void world_init(World *world)
{
    world->speed = 0.3f;
    world->pos_remainder = 0;
    world->scrolling_paused = 100;

    int index = 0;
    Image image = LoadImage("resources/tile_0.png");
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

    for (int image_index = 0; image_index < index; ++image_index)
        ImageFormat(world->images + image_index, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

    world->current_bitmap = LoadImageColors(image);
    world->next_bitmap = LoadImageColors(world->images[1]);
    world->screen_texture = LoadTextureFromImage(world->images[0]);

    index = 0;
    index = world->number_of_bg++;
    Vector2 origin = {0, 1};
    sprite_init_static_with_origin(&world->bg[index], "resources/forest/DeadForest_BG_0.png", 0, 350, origin);
    index = world->number_of_bg++;
    sprite_init_static_with_origin(&world->bg[index], "resources/forest/DeadForest_BG_0.png", 640, 350, origin);
    index = world->number_of_bg++;
    sprite_init_static_with_origin(&world->bg[index], "resources/forest/DeadForest_BG_1.png", 0, 350, origin);
    index = world->number_of_bg++;
    sprite_init_static_with_origin(&world->bg[index], "resources/forest/DeadForest_BG_1.png", 640, 350, origin);
    index = world->number_of_bg++;
    sprite_init_static_with_origin(&world->bg[index], "resources/forest/DeadForest_BG_2.png", 0, 350, origin);
    index = world->number_of_bg++;
    sprite_init_static_with_origin(&world->bg[index], "resources/forest/DeadForest_BG_2.png", 640, 350, origin);
    index = world->number_of_bg++;
    sprite_init_static_with_origin(&world->bg[index], "resources/forest/DeadForest_BG_3.png", 0, 350, origin);
    index = world->number_of_bg++;
    sprite_init_static_with_origin(&world->bg[index], "resources/forest/DeadForest_BG_3.png", 640, 350, origin);
    index = world->number_of_bg++;
    origin.x = 0.5;
    sprite_init_static_with_origin(&world->bg[index], "resources/forest/DeadForest_BG_4.png", WIDTH / 2, 350, origin);

    index = 0;
    origin.y = 0.5;
    index = world->number_of_fg++;
    sprite_init_static_with_origin(&world->fg[index], "resources/forest/fg1.png", WIDTH / 2, 350, origin);
    // sprite_init(&world->bg[index], "resources/forest/fg1.png", 960, 1, WIDTH / 2, HEIGHT / 2 + 290, 1, 0);

    // index = world->number_of_bg++;
    // sprite_init(&world->bg[index], "resources/forest/DeadForest_BG_3.png",640, 1, WIDTH / 2, HEIGHT / 2 + 90, 1, 0);

    world->shader = LoadShader(0, "resources/shader.fs");
    world->shader_position_location = GetShaderLocation(world->shader, "scroll_position");
    world->shader_map_location = GetShaderLocation(world->shader, "texture_map");
    world->map_texture = LoadTextureFromImage(LoadImage("resources/tileMap.png"));

    // init roots
    for (int i = 0; i < NUM_SEEDS; ++i)
    {
        int x = rand() % WIDTH;
        int y = 350;
        world_set_terrain(world, x, y, TERRA_ROOT_TIP);
        for (int offset = -3; offset < 3; ++offset)
        {
            // world->bitmap[POS(x + offset, y)] = TERRA_STONE;
        }
    }

    // init worms
    worms_init(&world->worms);
}

Color *
world_get_terrain(World *world, int x, int y)
{
    Color *bitmap = world->current_bitmap;
    int scroll_amount = world->current_scroll;
    int from_the_top = HEIGHT - scroll_amount;
    if (y >= from_the_top)
    {
        y -= from_the_top;
        bitmap = world->next_bitmap;
    }
    else
    {
        y += world->current_scroll;
    }
    if (x < 0 || y < 0 || x >= WIDTH || y >= HEIGHT)
        return &TERRA_EARTH;
    return bitmap + POS(x, y);
}

Dig world_check_dig(World *world, int x, int y, int radius)
{
    Dig dig = {0};
    for (int offsetX = -radius; offsetX < radius; ++offsetX)
    {
        for (int offsetY = -radius; offsetY < radius; ++offsetY)
        {
            if (sqrtf(offsetX * offsetX + offsetY * offsetY) <= radius)
            {
                Color *c = world_get_terrain(world, x + offsetX, y + offsetY);
                TerrainType t = color_to_terrain_type(*c);
                dig.types[t] += 1;
            }
        }
    }
    dig.types[TUNNEL] = 0;
    return (dig);
}

Dig world_dig(World *world, int x, int y, int radius)
{
    Dig dig = {0};
    for (int offsetX = -radius; offsetX < radius; ++offsetX)
    {
        for (int offsetY = -radius; offsetY < radius; ++offsetY)
        {
            if (sqrtf(offsetX * offsetX + offsetY * offsetY) <= radius)
            {
                Color *c = world_get_terrain(world, x + offsetX, y + offsetY);
                TerrainType t = color_to_terrain_type(*c);
                if (t == TerrainTypeSize) {
                    printf("ERROR: INVALID TERRAIN TYPE!");
                }
                dig.types[t] += 1;
                world_set_terrain(world, x + offsetX, y + offsetY, TERRA_TUNEL);
            }
        }
    }
    dig.types[TUNNEL] = 0;
    return (dig);
}

int world_scroll(World *world)
{
    if (world->scrolling_paused > 0)
    {
        --world->scrolling_paused;
    }
    else
    {
        world->pos_remainder += world->speed;
    }

    int lines_to_scroll = world->pos_remainder;
    for (int i = 0; i < lines_to_scroll; i++)
    {
        Sprite *bg;
        for (int i = 0; i < world->number_of_bg; i++)
        {
            bg = &world->bg[i];
            bg->position.y -= i * 0.05;
        }
        for (int i = 0; i < world->number_of_fg; i++)
        {
            bg = &world->fg[i];
            bg->position.y -= 1;
        }

        world->pos_remainder -= 1;
        world->current_scroll += 1;
        world->depth += 1;
        if (world->current_scroll == HEIGHT)
        {
            world->current_scroll = 0;
            UnloadImageColors(world->current_bitmap);
            world->current_bitmap = world->next_bitmap;
            int selected_image = rand() % world->number_of_images;
            world->next_bitmap = LoadImageColors(world->images[selected_image]);
        }
    }
    return lines_to_scroll;
}

void update_roots(World *world)
{
    int alternate = -1;

    // update pixles
    for (int x = 0; x < WIDTH; x++)
        for (int y = HEIGHT - 1; y > 0; y--)
        {
            alternate *= -1;

            Color *current = world_get_terrain(world, x, y);
            if (current->r == TERRA_ROOT_TIP.r)
            {
                // int targetHeight = x < (WIDTH / 3)
                //                        ? world->leftSpeed
                //                    : (x < 2 * WIDTH / 3)
                //                        ? world->centerSpeed
                //                        : world->leftSpeed;

                // if (rand() % targetHeight < y)
                //     continue;
                if (utils_random_int(0,100) > 25)
                    continue;

                unsigned char direction = current->g & 7;
                unsigned char age = current->g >> 3;

                if (age > utils_random_int(15,20))
                {
                    age = 0;
                    direction = utils_random_int(0,7);
                }
                age++;

                if ((utils_random_int(0,1000)) < 5)
                {
                    world_set_terrain(world, x, y, TERRA_ROOT_KNOT);
                }
                else
                {
                    world_set_terrain(world, x, y, TERRA_ROOT);
                }

                int r = utils_random_int(0,1000);
                r -= 300;
                r += 600 * (float)direction / 7;

                Color new_tip = TERRA_ROOT_TIP;
                new_tip.g = (age << 3) | direction;

                if (r > 333 && r < 666)
                {
                    world_set_terrain(world, x, y + 1, new_tip);
                }
                else if ((r < 666 && x > 0) || x >= WIDTH - 1)
                {
                    world_set_terrain(world, x - 1, y + 1, new_tip);
                }
                else
                {
                    world_set_terrain(world, x + 1, y + 1, new_tip);
                }
            }
            else if (current->r == TERRA_ROOT_KNOT.r)
            {
                if (utils_random_int(0,100) % 100 > 5)
                    continue;

                if (utils_random_int(0,100) < 2)
                {
                    world_set_terrain(world, x, y, TERRA_ROOT_KNOT);
                }
                else
                {
                    world_set_terrain(world, x, y, TERRA_ROOT);
                }

                if (utils_random_int(0,100) < 40)
                {
                    world_set_terrain(world, x, y + 1, TERRA_ROOT_TIP);
                }
                else if ((utils_random_int(0,100) % 1 < 1 && x > 0) || x >= WIDTH - 1)
                {
                    world_set_terrain(world, x - 1 * alternate, y + 1, TERRA_ROOT_TIP);
                }
                else
                {
                    world_set_terrain(world, x + 1 * alternate, y + 1, TERRA_ROOT_TIP);
                }
            }
            else if (current->r == TERRA_ROOT.r && current->a > 0)
            {
                int age = current->b;
                Color new_color = TERRA_ROOT;
                new_color.b = Clamp(current->b + 1, 0, 254);

                world_set_terrain(world, x, y, new_color);

                if (age == 150)
                {
                    Color *right = world_get_terrain(world, x + 1, y);
                    if (right->r > 3 || right->b < age)
                    {
                        world_set_terrain(world, x + 1, y, TERRA_ROOT);
                    }
                }
                if (age == 200)
                {
                    Color *left = world_get_terrain(world, x - 1, y);
                    if (left->r > 3 || left->b < age)
                    {
                        world_set_terrain(world, x - 1, y, TERRA_ROOT);
                    }
                }
            }
        }
}

void world_update(World *world)
{
    world->last_scroll = world_scroll(world);
    update_roots(world);
    worms_update(&world->worms, world->last_scroll);
}

void world_draw(World *world)
{

    for (int i = 0; i < world->number_of_bg; i++)
    {
        sprite_draw(&world->bg[i]);
    }

    Rectangle srcRect = {0, 0, WIDTH, HEIGHT};
    Rectangle dstRect = {0, 0, GetScreenWidth(), GetScreenHeight()};
    Vector2 origin = {0, 0};

    Rectangle upper_screen = {0, 0, WIDTH, HEIGHT - world->current_scroll};
    Rectangle lower_screen = {0, HEIGHT - world->current_scroll, WIDTH, world->current_scroll};
    UpdateTextureRec(world->screen_texture, upper_screen, world->current_bitmap + (world->current_scroll * WIDTH));
    UpdateTextureRec(world->screen_texture, lower_screen, world->next_bitmap);
    BeginShaderMode(world->shader);
    SetShaderValueTexture(world->shader, world->shader_map_location, world->map_texture);
    float scroll = world->current_scroll / (float)HEIGHT;
    SetShaderValue(world->shader, world->shader_position_location, &scroll, SHADER_UNIFORM_FLOAT);
    DrawTexturePro(world->screen_texture, srcRect, dstRect, origin, 0.0f, WHITE);
    EndShaderMode();

    // worms_draw(&world->worms);

    for (int i = 0; i < world->number_of_fg; i++)
    {
        sprite_draw(&world->fg[i]);
    }
}
