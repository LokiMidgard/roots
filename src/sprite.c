#include "raylib.h"
#include "raymath.h"
#include "sprite.h"
#include "config.h"

void sprite_init(Sprite *s, const char *file, int frame_width, int num_of_frames, int x, int y, int speed, float rotation)
{
    s->counter = 0;
    s->image = LoadTexture(file);
    SetTextureFilter(s->image, TEXTURE_FILTER_BILINEAR);
    s->number_of_frames = num_of_frames;
    s->frame_width = frame_width;
    s->position.x = x;
    s->position.y = y;
    s->speed = speed;
    s->rotation = rotation;
    s->tint = WHITE;
    s->origin.x = s->frame_width / 2;
    s->origin.y = s->image.height / 2;
}
void sprite_init_static_with_origin(Sprite *s, const char *file, int x, int y, Vector2 origin)
{
    s->counter = 0;
    s->image = LoadTexture(file);
    s->number_of_frames = 0;
    s->frame_width = s->image.width;
    s->position.x = x;
    s->position.y = y;
    s->speed = 0;
    s->rotation = 0;
    s->tint = WHITE;
    s->origin.x = s->frame_width * origin.x;
    s->origin.y = s->image.height * origin.y;
}

void sprite_draw(Sprite *s)
{
    // calculate current frame
    //int current_frame = (s->number_of_frames > 0 && s->speed > 0) ? s->counter / s->speed : 0;
    int frame_width = s->frame_width;
    int frame_height = s->image.height;
    Rectangle frame = {s->current_frame * frame_width, 0, frame_width, frame_height};
    Rectangle dstRect = {
        s->position.x,   // x
        s->position.y, // y
        frame_width,     // width
        frame_height   // height
    };

    DrawTexturePro(s->image, frame, dstRect, s->origin, s->rotation, s->tint);
}

void sprite_update(Sprite *s, Vector2 movement)
{
    s->counter += 1;
    if (s->counter > s->speed && s->speed > 0)
    {
        s->counter = 0;
        s->current_frame = (s->current_frame + 1) % s->number_of_frames;
    }
    //s->counter = (s->counter + 1) % (s->number_of_frames * s->speed);
    if (movement.x != 0 || movement.y != 0)
    {
        Vector2 one = {0, 0};
        s->rotation = Vector2Angle(one, movement) * RAD2DEG + 90;
        s->position = Vector2Add(s->position, movement);
    }
}
