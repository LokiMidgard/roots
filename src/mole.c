#include "raylib.h"
#include "raymath.h"
#include "config.h"
#include "mole.h"

void mole_init(Mole *mole, float x, float y)
{
    mole->snd_dig = LoadSound("resources/dig01.wav");
    SetSoundVolume(mole->snd_dig, 0.1f);
    mole->snd_collide = LoadSound("resources/crumble.wav");
    SetSoundVolume(mole->snd_collide, 0.5f);
    mole->snd_explode = LoadSound("resources/explode.wav");

    mole->health = 100;
    mole->points = 0;
    mole->speed = 3;
    sprite_init(&mole->sprite, "resources/mole.png", 8, 30, 30, 15, 0);

    Vector2 position = {x, y};
    mole->sprite.position = position;

    mole->speedBonus = 0;
    mole->stoneEaterBonus = 0;

    mole->explode_time = 0;
    particles_init(&mole->part_dig);
}

void mole_update(Mole *mole, Vector2 *movement)
{
    Sprite *sprite = &mole->sprite;

    // dig
    int mole_width = (sprite->image.width / sprite->number_of_frames);
    int mole_height = sprite->image.height;

    // set movement speed of mole
    *movement = Vector2Normalize(*movement);
    *movement = Vector2Scale(*movement, mole->speed);

    Vector2 new_mole_position = Vector2Add(sprite->position, *movement);

    float terain_multiplyer = 1.0f;
    for (int offsetX = -mole_width / 2; offsetX < mole_width / 2; ++offsetX)
        for (int offsetY = -mole_height / 2; offsetY < mole_height / 2; ++offsetY)
        {
            if ((offsetY != -mole_height / 2 && offsetY != mole_height / 2 - 1) || (offsetX != -mole_width / 2 && offsetX != mole_width / 2 - 1))
            {
                Color *current = world_get_terrain(&world, (int)new_mole_position.x + offsetX, (int)new_mole_position.y + offsetY);
                if (IS_COLOR(current, TERRA_ROOT))
                {
                    mole->health -= 1;
                }
                else if (IS_COLOR(current, TERRA_EMERALD))
                {
                    mole->points += 1;
                }
                else if (IS_COLOR(current, TERRA_QUICK_STONE))
                {
                    mole->speedBonus = 300;
                }
                else if (IS_COLOR(current, TERRA_DIG_STONE))
                {
                    mole->stoneEaterBonus = 600;
                }
            }
        }

    Dig dig = world_dig(&world, sprite->position.x, sprite->position.y, mole_width);
    for(int t = EARTH; t < TerrainTypeSize; ++t) {
        int num = (int)(dig.types[t] * 0.1f);
        if (num > 0) {
            particles_emit(&mole->part_dig, num, sprite->position.x, sprite->position.y, TerrainTypeToColor[t]);
        }
    }

    terain_multiplyer -= dig.types[EARTH] * 0.01f;
    terain_multiplyer -= dig.types[STONE] * 0.03f;
    terain_multiplyer = fmaxf(0.1f, terain_multiplyer);
    // calculate bonuses
    if (mole->speedBonus > 0)
    {
        *movement = Vector2Scale(*movement, 1.5);
        mole->speedBonus -= 1;
    }
    if (mole->stoneEaterBonus > 0)
    {
        mole->stoneEaterBonus -= 1;
    }
    *movement = Vector2Scale(*movement, terain_multiplyer);

    if (Vector2Length(*movement) > 0.5)
    {
        if (!IsSoundPlaying(mole->snd_dig))
        {
            PlaySound(mole->snd_dig);
        }
    }

    if (terain_multiplyer == 0)
    {
        if (!IsSoundPlaying(mole->snd_collide))
        {
            PlaySound(mole->snd_collide);
        }
    }

    if (mole->explode_time > 0) {
        mole->explode_time -= 0.09;
        if (mole->explode_time <= 0) {
            mole->explode_time = 0;
        }
        float radius = sin(mole->explode_time) * 50;
        Dig d = world_dig(&world, sprite->position.x, sprite->position.y, radius);
        for(int t = EARTH; t < TerrainTypeSize; ++t) {
            int num = (int)(d.types[t] * 0.1f);
            if (num > 0) {
                particles_emit(&mole->part_dig, num, sprite->position.x, sprite->position.y, TerrainTypeToColor[t]);
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
