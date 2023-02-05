#include "inventory.h"

void inventory_init(Inventory* inventory) {
    inventory->snd_pickup = LoadSound("resources/pickup.wav");
    SetSoundVolume(inventory->snd_pickup, 0.3f);
}

void inventory_add(Inventory* inventory, StuffType picked_up_type, int num) {
    inventory->pickups[picked_up_type] += num;
    if (!IsSoundPlaying(inventory->snd_pickup)) {
        SetSoundPitch(inventory->snd_pickup, 0.7 + (rand() % 60 / 100.0f));
        PlaySound(inventory->snd_pickup);
    }
}
