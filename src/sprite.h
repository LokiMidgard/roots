#ifndef ROOTS_SPRITE_H
#define ROOTS_SPRITE_H

#include "raylib.h"

typedef struct Sprite
{
    Vector2 position;
    int speed;
    int counter;
    Texture2D image;
    int number_of_frames;
    int frame_width;
    float rotation;
    Color tint;
} Sprite;

void sprite_init(Sprite *s, const char *file, int frame_width, int num_of_frames, int x, int y, int speed, float rotation);
void sprite_update(Sprite *s, Vector2 *movement);
void sprite_draw(Sprite *s);

#endif
