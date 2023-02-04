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
    bool collide = false;
    for (int offsetX = -mole_width / 2; offsetX < mole_width / 2; ++offsetX)
        for (int offsetY = -mole_height / 2; offsetY < mole_height / 2; ++offsetY)
        {
            if ((offsetY != -mole_height / 2 && offsetY != mole_height / 2 - 1) || (offsetX != -mole_width / 2 && offsetX != mole_width / 2 - 1))
            {
                Color c = bitmap[POS((int)new_mole_position.x + offsetX, (int)new_mole_position.y + offsetY)];
                if (c.r == TERRA_STONE.r)
                {
                    collide = true;
                }
            }
        }
    if (collide)
    {
        *movement = Vector2Zero();
    }
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