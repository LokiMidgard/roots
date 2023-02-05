#include "inventory.h"

void inventory_reset(Inventory* inventory)
{
    for (int index = 0; index < st_size; ++index)
    {
        inventory->pickups[index] = 0;
    }
}

void inventory_init(Inventory* inventory) {
    inventory->sounds[st_BOMB] = LoadSoundEx("resources/speech/bomb.wav", 3.0f);
    inventory->sounds[st_STAR] = LoadSoundEx("resources/speech/star.wav", 3.0f);
    inventory->sounds[st_MEAT] = LoadSoundEx("resources/speech/meat.wav", 3.0f);
    inventory->sounds[st_APPLE] = LoadSoundEx("resources/speech/apple.wav", 3.0f);
    inventory->sounds[st_BEATLE] = LoadSoundEx("resources/speech/beetle.wav", 3.0f);
    inventory->snd_pickup = LoadSoundEx("resources/sound/pickup.wav", 0.3f);
}

void inventory_add(Inventory* inventory, StuffType picked_up_type, int num) {
    inventory->pickups[picked_up_type] += num;
    Sound s;
    switch(picked_up_type) {
        case st_BOMB:
        case st_STAR:
        case st_MEAT:
        case st_APPLE:
        case st_BEATLE:
            s = inventory->sounds[picked_up_type];
            break;
        default:
            s = inventory->snd_pickup;
            SetSoundPitch(inventory->snd_pickup, 0.7 + (rand() % 60 / 100.0f));
            break;
    }
    if (!IsSoundPlaying(s)) {
        PlaySound(s);
    }
}

bool inventory_use(Inventory* inventory, StuffType type) {
    if (inventory->pickups[type] > 0) {
        --inventory->pickups[type];
        switch(type) {
            case st_STAR:
            case st_MEAT:
            case st_APPLE:
            case st_BEATLE:
                PlaySound(inventory->sounds[type]);
                break;
        }
        return true;
    }
    return false;
}
