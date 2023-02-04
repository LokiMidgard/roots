#include "config.h"
#include "raylib.h"

typedef struct Sprite
{
    Vector2 position;
    int speed;
    int counter;
    Texture2D image;
    int number_of_frames;
} Sprite;

Sprite* sprite_new(const char* file, int num_of_frames, int x, int y, int speed)
{
    Sprite* s = malloc(sizeof(Sprite));
    s->counter = 0;
    s->image = LoadTexture(file);
    s->number_of_frames = num_of_frames;
    s->position.x = x;
    s->position.y = y;
    s->speed = speed;
    return s;
}

void sprite_draw(Sprite *s)
{
    // calculate current frame
    int current_frame = s->counter / s->speed;
    int frame_width = s->image.width / s->number_of_frames;
    int frame_height = s->image.height;
    Rectangle frame = {current_frame * frame_width, 0, frame_width, frame_height};
    Rectangle dstRect = {s->position.x * GetScreenWidth() / WIDTH, s->position.y * GetScreenHeight() / HEIGHT, frame_width * GetScreenWidth() / WIDTH, frame_height * GetScreenHeight() / HEIGHT};
    Vector2 origin = {0, 0};
    DrawTexturePro(s->image, frame, dstRect, origin, 0.0f, WHITE);
}

void sprite_update(Sprite *s)
{
    s->counter = (s->counter + 1) % (s->number_of_frames * s->speed);
}
