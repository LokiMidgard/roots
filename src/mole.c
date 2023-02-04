#include "raylib.h"
#include "raymath.h"
#include "config.h"
#include "mole.h"

void mole_update(Sprite *mole, Vector2 *movement, Color *bitmap)
{
    // dig
    int mole_width = (mole->image.width / mole->number_of_frames);
    int mole_height = mole->image.height;
    Vector2 new_mole_position = Vector2Add(mole->position, *movement);
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
                bitmap[POS((int)mole->position.x + offsetX, (int)mole->position.y + offsetY)] = TERRA_TUNEL;
        }

    sprite_update(mole, movement);

    // ensure position is in bounds
    if (mole->position.x < mole->image.width / mole->number_of_frames)
    {
        mole->position.x = mole->image.width / mole->number_of_frames;
    }
    if (mole->position.y < mole->image.height)
    {
        mole->position.y = mole->image.height;
    }

    if (mole->position.x > WIDTH - mole->image.width / mole->number_of_frames)
    {
        mole->position.x = WIDTH - mole->image.width / mole->number_of_frames;
    }
    if (mole->position.y > HEIGHT - mole->image.height)
    {
        mole->position.y = HEIGHT - mole->image.height;
    }
}