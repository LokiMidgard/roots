#include "config.h"
#include "worms.h"
#include "raylib.h"

void init_worm(Worms* worms, int x, int y) {
    for(int i=0; i<MAX_WORMS; ++i) {
        if (!worms->alive[i]) {
            worms->alive[i] = true;
            worms->pos[i].x = x;
            worms->pos[i].y = y;
            return;
        }
    }
}

void worms_init(Worms* worms) {
    worms->tex = LoadTexture("resources/stuff.png");
    for (int i=0; i<3; ++i) {
        init_worm(worms, 30 + (rand() % (WIDTH-60)), HEIGHT - (rand() % 50));
    }
}

void worms_update(Worms* worms, int last_scroll) {
    int alive = 0;
    for(int i=0; i<MAX_WORMS; ++i) {
        if (worms->alive[i]) {
            ++alive; 
            worms->pos[i].y -= last_scroll;
        }
    }
    if (alive < 3) {
        init_worm(worms, 100, 100);
    }
}

void worms_draw(Worms* worms) {
    for(int i=0; i<MAX_WORMS; ++i) {
        if (worms->alive[i]) {
            Rectangle srcRect = { 0, 0, 8, 8};
            Rectangle dstRect = {
                worms->pos[i].x,     // x
                worms->pos[i].y,   // y
                8,                   // width
                8                  // height
            };
            Vector2 origin = { 4, 4};
            DrawTexturePro(worms->tex, srcRect, dstRect, origin, 0, WHITE);
        }
    }
}
