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
    mole->health = 100;
    mole->points = 0;
    mole->speed = 3;
    sprite_init(&mole->sprite, "resources/mole.png", 8, 30, 30, 15, 0);

    Vector2 position = {x, y};
    mole->sprite.position = position;

    mole->speedBonus = 0;
    mole->stoneEaterBonus = 0;
}

void mole_update(Mole *mole, Vector2 *movement, Color *bitmap)
{
    Sprite *sprite = &mole->sprite;

    // dig
    int mole_width = (sprite->image.width / sprite->number_of_frames);
    int mole_height = sprite->image.height;

    // set movement speed of mole
    *movement = Vector2Normalize(*movement);
    *movement = Vector2Scale(*movement, mole->speed);

    Vector2 new_mole_position = Vector2Add(sprite->position, *movement);

    if (Vector2Length(*movement) > 0.5)
    {
        if (!IsSoundPlaying(mole->snd_dig))
        {
            PlaySound(mole->snd_dig);
        }
    }

    float terain_multiplyer = 1.0f;
    for (int offsetX = -mole_width / 2; offsetX < mole_width / 2; ++offsetX)
        for (int offsetY = -mole_height / 2; offsetY < mole_height / 2; ++offsetY)
        {
            if ((offsetY != -mole_height / 2 && offsetY != mole_height / 2 - 1) || (offsetX != -mole_width / 2 && offsetX != mole_width / 2 - 1))
            {
                Color *current = &bitmap[POS((int)new_mole_position.x + offsetX, (int)new_mole_position.y + offsetY)];
                if (IS_COLOR(current, TERRA_STONE))
                {
                    terain_multiplyer = 0.15f;
                }
                else if (IS_COLOR(current, TERRA_EARTH))
                {
                    terain_multiplyer = fmin(terain_multiplyer, 0.5f);
                }
            }
        }

    *movement = Vector2Scale(*movement, terain_multiplyer);
    // digging
    if (terain_multiplyer == 0)
    {
        if (!IsSoundPlaying(mole->snd_collide))
        {
            PlaySound(mole->snd_collide);
        }
    }

    for (int offsetX = -mole_width / 2; offsetX < mole_width / 2; ++offsetX)
        for (int offsetY = -mole_height / 2; offsetY < mole_height / 2; ++offsetY)
        {
            if ((offsetY != -mole_height / 2 && offsetY != mole_height / 2 - 1) || (offsetX != -mole_width / 2 && offsetX != mole_width / 2 - 1))
            {
                bitmap[POS((int)sprite->position.x + offsetX, (int)sprite->position.y + offsetY)] = TERRA_TUNEL;
            }
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
}

void mole_draw(Mole *mole)
{
    sprite_draw(&mole->sprite);
}
