#ifndef STUFF_H
#define STUFF_H

#include "raylib.h"
#include "world.h"

#define MAX_STUFF 20

typedef enum StuffType
{
    st_STAR,
    st_BOMB,
    st_MEAT,
    st_APPLE,
    st_size,
} StuffType;

char *StuffTypeName[st_size] =
{
    "Star",
    "Bomb",
    "Meat",
    "Apple",
};

typedef struct Stuff
{
    Texture2D  tex;
    int        active_stuff;
    StuffType  type[MAX_STUFF];
    Vector2    pos[MAX_STUFF];
    int        spawn_timer[st_size];
    int        random_spawn_ranges_start[st_size];
    int        random_spawn_ranges_end[st_size];
} Stuff;

void stuff_init(Stuff* stuff);
void stuff_spawn(Stuff* stuff, int x, int y, StuffType type);
void stuff_update(Stuff* stuff, int last_scroll);
void stuff_draw(Stuff* stuff);

#endif
