#include "config.h"
#include "stuff.h"
#include "raylib.h"

void spawn_stuff(Stuff* stuff, int x, int y, StuffType type) {
    if (stuff->active_stuff == MAX_STUFF)
        return;
    int index = stuff->active_stuff++;
    stuff->pos[index].x = x;
    stuff->pos[index].y = y;
    stuff->type[index] = type;

    world_dig(&world, x-4, y-4, 12);
}

void stuff_init(Stuff* stuff) {
    stuff->tex = LoadTexture("resources/sprites/stuff.png");
    stuff->random_spawn_ranges_start[st_STAR] = 60 * 60;
    stuff->random_spawn_ranges_end[st_STAR] = 100 * 60;
    stuff->random_spawn_ranges_start[st_BOMB] = 10 * 60;
    stuff->random_spawn_ranges_end[st_BOMB] = 20 * 60;
    stuff->random_spawn_ranges_start[st_MEAT] = 30 * 60;
    stuff->random_spawn_ranges_end[st_MEAT] = 120 * 60;
    stuff->random_spawn_ranges_start[st_APPLE] = 1 * 60;
    stuff->random_spawn_ranges_end[st_APPLE] = 100 * 60;
}

void stuff_remove(Stuff *stuff, int index)
{
    if (stuff->active_stuff == 1)
    {
        stuff->active_stuff = 0;
        return;
    }
    int last_item = stuff->active_stuff - 1;
    stuff->pos[index] = stuff->pos[last_item];
    stuff->type[index] = stuff->type[last_item];
    stuff->active_stuff -= 1;
}


void stuff_update(Stuff* stuff, int last_scroll) {
    // update spawn timer
    for (int index = 0; index < st_size; ++index)
    {
        stuff->spawn_timer[index] -= 1;
        if (stuff->spawn_timer[index] <= 0)
        {
            StuffType type = rand() % st_size;
            spawn_stuff(stuff, 30 + (rand() % (WIDTH-60)), HEIGHT + (rand() % 50), type);
            stuff->spawn_timer[index] = utils_random_int(
                stuff->random_spawn_ranges_start[type],
                stuff->random_spawn_ranges_end[type]);
        }
    }

    for (int index = 0; index < stuff->active_stuff; ++index)
    {
        stuff->pos[index].y -= last_scroll;
        if (stuff->pos[index].y < -10)
        {
            stuff_remove(stuff, index);
            --index;
        }
    }
}

int stuff_pickup(Stuff *stuff, Vector2 position, float pickup_radius, StuffType *type)
{
    for (int index = 0; index < stuff->active_stuff; ++index)
    {
        if (Vector2Length(Vector2Subtract(position, stuff->pos[index])) < pickup_radius)
        {
            if (type)
                *type = stuff->type[index];
            stuff_remove(stuff, index);
            return 1;
        }
    }
    return 0;
}

void stuff_draw(Stuff* stuff) {
    for(int index = 0; index < stuff->active_stuff; ++index)
    {
        StuffType type = stuff->type[index];
        int source_start_x = type * 8;
        Rectangle srcRect = {source_start_x + 0.5f, 0.5f, 7.0f, 7.0f};
        float offset = (1.0 + sin(GetTime() * 6));
        Rectangle dstRect = {
            (stuff->pos[index].x-4) - offset,     // x
            (stuff->pos[index].y-4) - offset,   // y
            8 + offset*2,                   // width
            8 + offset*2                  // height
        };
        Vector2 origin = {4, 4};
        DrawTexturePro(stuff->tex, srcRect, dstRect, origin, 0, WHITE);
    }
}
