#include "raylib.h"
#include "raymath.h"
#include "config.h"
#include "mole.h"
#include "world.h"
#include "utils.h"

void mole_reset(Mole *mole, int x, int y)
{
    mole->snd_dig = mole->snd_dig_earth;
    mole->health = MOLE_MAX_HEALTH;
    mole->points = 0;
    mole->speed = 100.0f * 0.015f;

    Vector2 position = {x, y};
    mole->sprite.position = position;

    mole->speedBonus = 0;
    mole->stoneEaterBonus = 0;

    mole->explode_time = 0;
}

void mole_init(Mole *mole, float x, float y)
{
    // mole->snd_dig_earth = LoadSoundEx("resources/sound/dig03.mp3", 0.7f);
    mole->snd_dig_earth = LoadSoundEx("resources/sound/dig01.wav", 0.1f);
    mole->snd_dig_stone = LoadSoundEx("resources/sound/dig_stone.mp3", 0.4f);
    mole->snd_collide = LoadSoundEx("resources/sound/crumble.wav", 0.5f);
    mole->snd_explode = LoadSoundEx("resources/sound/explode.wav", 1.2f);
    mole->snd_hurt = LoadSoundEx("resources/sound/deep.wav", 1.0f);
    mole->snd_heal = LoadSoundEx("resources/sound/powerup.wav", 0.5f);
    mole->snd_speed = LoadSoundEx("resources/sound/speed.wav", 0.3f);
    mole->snd_powerup = LoadSoundEx("resources/sound/powerup.wav", 0.3f);

    sprite_init(&mole->sprite, "resources/sprites/mole_test.png", 16, 5, 30, 30, 15, 0);

    particles_init(&mole->part_dig);

    mole_reset(mole, x, y);
}

void mole_update(Mole *mole, Vector2 movement)
{
    Sprite *sprite = &mole->sprite;

    int mole_width = 6;

    movement = Vector2Normalize(movement);
    movement = Vector2Scale(movement, mole->speed);

    Vector2 new_mole_position = Vector2Add(sprite->position, movement);

    Dig dig = world_check_dig(&world,
                              new_mole_position.x,
                              new_mole_position.y,
                              mole_width);

    // Points
    mole->points += dig.types[EMERALD] * 20 * 0.003f;
    mole->points += dig.types[EARTH] * 1 * 0.003f;
    mole->points += dig.types[STONE] * 2 * 0.003f;


    float dig_speed_penalty = 0.0f;
    dig_speed_penalty += dig.types[EARTH] * 0.02f;
    dig_speed_penalty += dig.types[STONE] * 0.05f;
    dig_speed_penalty += dig.types[EMERALD] * 0.05f;
    dig_speed_penalty += dig.types[QUICK_STONE] * 0.05f;
    dig_speed_penalty += dig.types[DIG_STONE] * 0.05f;
    float max_dig_speed_penalty = mole->stoneEaterBonus > 0 ? 0.48f : 0.98f;
    dig_speed_penalty = fminf(max_dig_speed_penalty, dig_speed_penalty);
    float defaultSpeed = mole->speedBonus > 0 ? 1.5f : 1.0f;
    movement = Vector2Scale(movement, (defaultSpeed - dig_speed_penalty));
    new_mole_position = Vector2Add(sprite->position, movement);

    dig = world_dig(&world,
                    new_mole_position.x,
                    new_mole_position.y,
                    mole_width);
    sprite->position = new_mole_position;
    if (dig.types[ROOT])
    {
        if (!IsSoundPlaying(mole->snd_hurt))
        {
            PlaySound(mole->snd_hurt);
        }
        particles_emit(&mole->part_dig, 20, sprite->position.x, sprite->position.y, RED, 1.4f);
    }
    mole->health -= dig.types[ROOT];
    mole->health -= dig.types[LAVA] * 0.03;

    for (int t = 0; t < TerrainTypeSize; ++t)
    {
        int num = (int)(dig.types[t] * 0.4f);
        if (num > 0)
        {
            particles_emit(&mole->part_dig, num, sprite->position.x, sprite->position.y, terrain_type_to_color(t), 1);
        }
    }

    // calculate bonuses
    if (mole->speedBonus > 0)
    {
        movement = Vector2Scale(movement, 1.5);
        mole->speedBonus -= 1;
    }
    if (mole->stoneEaterBonus > 0)
    {
        mole->stoneEaterBonus -= 1;
    }

    if (dig.types[STONE] + dig.types[DIG_STONE] + dig.types[QUICK_STONE] + dig.types[EMERALD] > dig.types[EARTH] + dig.types[SAND])
        mole->snd_dig = mole->snd_dig_stone;
    else
        mole->snd_dig = mole->snd_dig_earth;

    if (fabsf(movement.x) > 0.0f || fabsf(movement.y) > 0.0f)
    {
        mole->sprite.speed = 5;
        if (!IsSoundPlaying(mole->snd_dig) && (dig.types[EARTH] > 1 || dig.types[STONE] > 1 || dig.types[EMERALD] > 1 || dig.types[SAND] > 1 || dig.types[QUICK_STONE] > 1 || dig.types[DIG_STONE] > 1))
            PlaySound(mole->snd_dig);
    }
    else
    {
        mole->sprite.speed = 0;
    }

    if (mole->explode_time > 0)
    {
        mole->explode_time -= 0.09;
        if (mole->explode_time <= 0)
        {
            mole->explode_time = 0;
        }
        float radius = sin(mole->explode_time) * 50;
        Dig d = world_dig(&world, sprite->position.x, sprite->position.y, radius);
        for (int t = EARTH; t < TerrainTypeSize; ++t)
        {
            int num = (int)(d.types[t] * 0.4f);
            if (num > 0)
            {
                particles_emit(&mole->part_dig, num, sprite->position.x, sprite->position.y, TerrainTypeToColor[t], 4);
            }
        }
    }

    if (mole->stoneEaterBonus > 0 && mole->speedBonus > 0)
    {
        sprite->tint = ORANGE;
    }
    else if (mole->speedBonus > 0)
    {
        sprite->tint = YELLOW;
    }
    else if (mole->stoneEaterBonus > 0)
    {
        sprite->tint = RED;
    }
    else
    {
        sprite->tint = WHITE;
    }
    sprite_update(sprite, movement);

    // ensure position is in bounds
    if (sprite->position.x < sprite->image.width / sprite->number_of_frames)
    {
        sprite->position.x = sprite->image.width / sprite->number_of_frames;
    }
    if (sprite->position.y < sprite->image.height)
    {
        sprite->position.y = sprite->image.height;
    }

    if (sprite->position.x > WIDTH - sprite->image.width / sprite->number_of_frames)
    {
        sprite->position.x = WIDTH - sprite->image.width / sprite->number_of_frames;
    }
    if (sprite->position.y > HEIGHT - sprite->image.height)
    {
        sprite->position.y = HEIGHT - sprite->image.height;
    }

    particles_update(&mole->part_dig);
}

void mole_draw(Mole *mole)
{
    particles_draw(&mole->part_dig);
    sprite_draw(&mole->sprite);
}

void mole_consume(Mole* mole, Inventory* inventory, StuffType type) {
    if (inventory_use(inventory, type)) {
        switch(type) {
            case st_APPLE:
                mole->health += 20;
                if (mole->health > MOLE_MAX_HEALTH) {
                    mole->health = MOLE_MAX_HEALTH;
                } else {
                    PlaySound(mole->snd_heal);
                }
                break;
            case st_MEAT:
                mole->stoneEaterBonus = 2*60;
                PlaySound(mole->snd_powerup);
                break;
            case st_STAR:
                mole->speedBonus = 5*60;
                PlaySound(mole->snd_speed);
                break;
            case st_BOMB:
                if (mole->explode_time == 0) {
                    mole->explode_time = 3.1415f;
                    if (!IsSoundPlaying(mole->snd_explode)) {
                        PlaySound(mole->snd_explode);
                    }
                }
                break;
            default:
                break;
        }
    }
}
