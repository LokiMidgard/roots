#include "raylib.h"
#include "raymath.h"
#include "config.h"
#include "mole.h"
#include "world.h"

void mole_init(Mole *mole, float x, float y)
{
    mole->snd_dig = LoadSound("resources/dig01.wav");
    SetSoundVolume(mole->snd_dig, 0.1f);
    mole->snd_collide = LoadSound("resources/crumble.wav");
    SetSoundVolume(mole->snd_collide, 0.5f);
    mole->snd_explode = LoadSound("resources/explode.wav");

    mole->health = 100;
    mole->points = 0;
    mole->speed = 100.0f * 0.015f;
    sprite_init(&mole->sprite, "resources/mole_test.png", 16, 16, 30, 30, 15, 0);

    Vector2 position = {x, y};
    mole->sprite.position = position;

    mole->speedBonus = 0;
    mole->stoneEaterBonus = 0;

    mole->explode_time = 0;
    particles_init(&mole->part_dig);
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
    float dig_speed_penalty = 0.0f;
    dig_speed_penalty += dig.types[EARTH] * 0.02f;
    dig_speed_penalty += dig.types[STONE] * 0.05f;
    float max_dig_speed_penalty = 0.98f;
    dig_speed_penalty = fminf(max_dig_speed_penalty, dig_speed_penalty);
    movement = Vector2Scale(movement, (1.0f - dig_speed_penalty));
    new_mole_position = Vector2Add(sprite->position, movement);

    dig = world_dig(&world,
                    new_mole_position.x,
                    new_mole_position.y,
                    mole_width);
    sprite->position = new_mole_position;
    mole->health -= dig.types[ROOT];

    for(int t = 0; t < TerrainTypeSize; ++t) {
        int num = (int)(dig.types[t] * 0.4f);
        if (num > 0) {
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

    if (Vector2Length(movement) > 0.5f)
    {
        if (!IsSoundPlaying(mole->snd_dig))
            PlaySound(mole->snd_dig);
    }

    if (dig_speed_penalty == max_dig_speed_penalty)
    {
        if (!IsSoundPlaying(mole->snd_collide))
            PlaySound(mole->snd_collide);
    }

    if (mole->explode_time > 0) {
        mole->explode_time -= 0.09;
        if (mole->explode_time <= 0) {
            mole->explode_time = 0;
        }
        float radius = sin(mole->explode_time) * 50;
        Dig d = world_dig(&world, sprite->position.x, sprite->position.y, radius);
        for(int t = EARTH; t < TerrainTypeSize; ++t) {
            int num = (int)(d.types[t] * 0.4f);
            if (num > 0) {
                particles_emit(&mole->part_dig, num, sprite->position.x, sprite->position.y, TerrainTypeToColor[t], 4);
            }
        }
    }

    if (mole->stoneEaterBonus > 0 && mole->speedBonus > 0)
    {
        sprite->tint = PURPLE;
    }
    else if (mole->speedBonus > 0)
    {
        sprite->tint = BLUE;
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
    sprite_draw(&mole->sprite);
    particles_draw(&mole->part_dig);
}

void mole_explode(Mole* mole) {
    if (mole->explode_time == 0) {
        mole->explode_time = 3.1415f;
        if (!IsSoundPlaying(mole->snd_explode)) {
            PlaySound(mole->snd_explode);
        }
    }
}
