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
}

void stuff_init(Stuff* stuff) {
    stuff->tex = LoadTexture("resources/stuff.png");
    for (int index = 0; index < 3; ++index)
    {
        StuffType type = rand() % st_size;
        spawn_stuff(stuff, 30 + (rand() % (WIDTH-60)), HEIGHT - (rand() % 50), type);
    }
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
    for(int index = 0; index < stuff->active_stuff; ++index)
    {
        stuff->pos[index].y -= last_scroll;
        if (stuff->pos[index].y < -10)
        {
            stuff_remove(stuff, index);
            --index;
        }
    }
    if (stuff->active_stuff < 3)
    {
        StuffType type = rand() % st_size;
        spawn_stuff(stuff, 30 + (rand() % (WIDTH-60)), HEIGHT - (rand() % 50), type);
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
        Rectangle dstRect = {
            (stuff->pos[index].x-4) * GetScreenWidth() / WIDTH,     // x
            (stuff->pos[index].y-4) * GetScreenHeight() / HEIGHT,   // y
            8 * GetScreenWidth() / WIDTH,                   // width
            8 * GetScreenHeight() / HEIGHT                  // height
        };
        Vector2 origin = {4, 4};
        DrawTexturePro(stuff->tex, srcRect, dstRect, origin, 0, WHITE);
    }
}
