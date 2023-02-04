#include "raylib.h"
#include "raymath.h"
#include "config.h"
#include "mole.h"

void mole_init(Mole *mole, float x, float y)
{
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
    // dig
    int mole_width = (mole->sprite.image.width / mole->sprite.number_of_frames);
    int mole_height = mole->sprite.image.height;
    // set movement speed of mole
    *movement = Vector2Normalize(*movement);
    *movement = Vector2Scale(*movement, mole->speed);

    Vector2 new_mole_position = Vector2Add(mole->sprite.position, *movement);

    float terain_multiplyer = 1.0f;
    for (int offsetX = -mole_width / 2; offsetX < mole_width / 2; ++offsetX)
        for (int offsetY = -mole_height / 2; offsetY < mole_height / 2; ++offsetY)
        {
            if ((offsetY != -mole_height / 2 && offsetY != mole_height / 2 - 1) || (offsetX != -mole_width / 2 && offsetX != mole_width / 2 - 1))
            {
                Color *current = &bitmap[POS((int)new_mole_position.x + offsetX, (int)new_mole_position.y + offsetY)];
                if (IS_COLOR(current, TERRA_STONE))
                {
                    terain_multiplyer = 0.0f;
                }
                else if (IS_COLOR(current, TERRA_EARTH))
                {
                    terain_multiplyer = fmin(terain_multiplyer, 0.5f);
                }
            }
        }

    printf("multipriye %f\n", terain_multiplyer);
    *movement = Vector2Scale(*movement, terain_multiplyer);

    for (int offsetX = -mole_width / 2; offsetX < mole_width / 2; ++offsetX)
        for (int offsetY = -mole_height / 2; offsetY < mole_height / 2; ++offsetY)
        {
            if ((offsetY != -mole_height / 2 && offsetY != mole_height / 2 - 1) || (offsetX != -mole_width / 2 && offsetX != mole_width / 2 - 1))
                bitmap[POS((int)mole->sprite.position.x + offsetX, (int)mole->sprite.position.y + offsetY)] = TERRA_TUNEL;
        }

    sprite_update(mole, movement);

    // ensure position is in bounds
    if (mole->sprite.position.x < mole->sprite.image.width / mole->sprite.number_of_frames)
    {
        mole->sprite.position.x = mole->sprite.image.width / mole->sprite.number_of_frames;
    }
    if (mole->sprite.position.y < mole->sprite.image.height)
    {
        mole->sprite.position.y = mole->sprite.image.height;
    }

    if (mole->sprite.position.x > WIDTH - mole->sprite.image.width / mole->sprite.number_of_frames)
    {
        mole->sprite.position.x = WIDTH - mole->sprite.image.width / mole->sprite.number_of_frames;
    }
    if (mole->sprite.position.y > HEIGHT - mole->sprite.image.height)
    {
        mole->sprite.position.y = HEIGHT - mole->sprite.image.height;
    }
}