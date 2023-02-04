#include "raylib.h"
#include "raymath.h"
#include "sprite.h"
#include "config.h"

void sprite_init(Sprite *s, const char *file, int frame_width, int num_of_frames, int x, int y, int speed, float rotation)
{
    s->counter = 0;
    s->image = LoadTexture(file);
    s->number_of_frames = num_of_frames;
    s->frame_width = frame_width;
    s->position.x = x;
    s->position.y = y;
    s->speed = speed;
    s->rotation = rotation;
    s->tint = WHITE;
}

void sprite_draw(Sprite *s)
{
    // calculate current frame
    int current_frame = s->counter / s->speed;
    int frame_width = s->frame_width;
    int frame_height = s->image.height;
    Rectangle frame = {current_frame * frame_width, 0, frame_width, frame_height};
    Rectangle dstRect = {
        s->position.x * GetScreenWidth() / WIDTH,   // x
        s->position.y * GetScreenHeight() / HEIGHT, // y
        frame_width * GetScreenWidth() / WIDTH,     // width
        frame_height * GetScreenHeight() / HEIGHT   // height
    };
    Vector2 origin = {frame_width / 2 * GetScreenWidth() / WIDTH, frame_height / 2 * GetScreenHeight() / HEIGHT};
    DrawTexturePro(s->image, frame, dstRect, origin, s->rotation, s->tint);
}

void sprite_update(Sprite *s, Vector2 *movement)
{
    s->counter = (s->counter + 1) % (s->number_of_frames * s->speed);
    if (movement->x != 0 || movement->y != 0)
    {
        Vector2 one = {0, -1};
        s->rotation = Vector2Angle(one, *movement) * RAD2DEG;
        s->position = Vector2Add(s->position, *movement);
    }
}
