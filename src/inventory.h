#ifndef ROOTS_INVENTORY_H
#define ROOTS_INVENTORY_H

#include "raylib.h"
#include "stuff.h"

typedef struct Inventory {
    int pickups[st_size];
    Sound snd_pickup;
} Inventory;

void inventory_init(Inventory* inventory);
void inventory_add(Inventory* inventory, StuffType picked_up_type, int num);

#endif