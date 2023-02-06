#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raylib.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#include "config.h"
#include "utils.h"
#include "console.h"
#include "screen.h"
#include "input.h"
#include "sprite.h"
#include "world.h"
#include "mole.h"
#include "worms.h"
#include "stuff.h"
#include "particles.h"
#include "hud.h"
#include "inventory.h"

Mole mole;
World world;
Screen screen;
Input input;
Stuff stuff;
Sprite lose;
Sprite continueS;
Hud hud;
Music music;
Font fontBm;
Sound snd_intro;
Sound snd_lost;
Inventory inventory;
int endScreenTimer;

bool started = false;
bool died = false;

#include "console.c"
#include "input.c"
#include "screen.c"
#include "sprite.c"
#include "world.c"
#include "mole.c"
#include "worms.c"
#include "stuff.c"
#include "particles.c"
#include "hud.c"
#include "inventory.c"

void reset_game()
{
    mole_reset(&mole, WIDTH / 2, HEIGHT + 60);
    world_reset(&world);
    inventory_reset(&inventory);
    endScreenTimer = 0;
    died = false;
}

void init_game()
{
    world_init(&world);
    hud_init(&hud, &inventory);
    screen_init(&screen);
    mole_init(&mole, WIDTH / 2, HEIGHT + 60);
    stuff_init(&stuff);
    inventory_init(&inventory);
}

void MainLoop()
{
    UpdateMusicStream(music);
    screen_update(&screen);

    Vector2 movement = input_get_dir(&input);

    if (IsKeyPressed(KEY_P)) {
        started = !started;
    }

    if (IsKeyPressed(KEY_R)) {
        world_reload_tilemap(&world);
    }

    if (started)
    {
        if (input_is_button_pressed(&input, 0)) // down
        {
            if (inventory_use(&inventory, st_MEAT))
            {
                mole.stoneEaterBonus = 2*60;
            }
        }

        if (input_is_button_pressed(&input, 1)) // right
        {
            if (inventory_use(&inventory, st_APPLE))
            {
                mole.health += 20;
                if (mole.health > MOLE_MAX_HEALTH)
                    mole.health = MOLE_MAX_HEALTH;
            }
        }

        if (input_is_button_pressed(&input, 2)) // up
        {
            if (inventory_use(&inventory, st_STAR))
            {
                mole.speedBonus = 5*60;
            }
        }
        if (input_is_button_pressed(&input, 3)) // left
        {
            if (inventory_use(&inventory, st_BOMB))
            {
                mole_explode(&mole);
            }
        }
    }

    if (started && mole.health > 0)
    {
        // update
        world_update(&world);
        mole.sprite.position.y -= world.last_scroll;
        mole_update(&mole, movement);

        snprintf(hud.debug_text, 256, "depth: %2.2d\npoints: %d\nhealth: %.f\ninput: %s\npause: P\nfullscreen: F\nexit: ESC", world.depth, (int)mole.points, mole.health, input_get_device_name(&input));
        hud_update(&hud);
        stuff_update(&stuff, world.last_scroll);

        if (mole.health <= 0) {
            if (!died) {
                PlaySound(snd_lost);
                died = true;
            }
        }
    }

    if (!started)
    {
        if (input_any(&input))
        {
            started = true;
        }
    } else {
        // check for collisions
        float pickup_radius = 10.0f;
        StuffType picked_up_type;
        int success = stuff_pickup(&stuff,
                                mole.sprite.position,
                                pickup_radius,
                                &picked_up_type);
        if (success)
            inventory_add(&inventory, picked_up_type, 1);
    }

    // draw
    BeginDrawing();
    BeginTextureMode(screen.buffer);
    ClearBackground(MAGENTA);
    world_draw(&world);
    stuff_draw(&stuff);
    mole_draw(&mole);
    hud_draw(&hud, &stuff);

    if (mole.health <= 0)
    {
        float border_x = 0;//WIDTH / 10.0f;
        float border_y = 0;//HEIGHT / 10.0f;
        Color background_color = {0, 0, 0, Clamp(endScreenTimer*1.5f,0,200)}; 
        DrawRectangle(border_x, border_y, WIDTH - 2 * border_x, HEIGHT - 2 * border_y, background_color);
        endScreenTimer++;
        if (endScreenTimer > 150)
        {
            continueS.tint = (Color){255,255,255,Clamp((endScreenTimer-150)*3.5f,0,255)};
            sprite_draw(&continueS);
        }
        if (input_any(&input) && endScreenTimer > 150)
        {
            reset_game();
        }
        lose.tint = (Color){255,255,255,Clamp(endScreenTimer*2.5f,0,255)};
        sprite_draw(&lose);
        char points[10];
        snprintf(points, 10, "%d", (int)mole.points);
        Vector2 measurment = MeasureTextEx(fontBm, points, (float)fontBm.baseSize, 2);
        DrawTextEx(fontBm, points, (Vector2){(WIDTH / 2 -measurment.x/2 ), (HEIGHT / 2-measurment.y/2 )}, (float)fontBm.baseSize, 2, (Color){255,255,255,Clamp(endScreenTimer*2.5f,0,255)});
    }
    EndTextureMode();
    ClearBackground(BLACK);
    screen_draw(&screen);
    EndDrawing();
}


int main()
{
    /***************************************************************************
     * Init technical stuff
     ****************************************************************************/
    InitConsole();
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "raylib");
    InitAudioDevice();
    SetTargetFPS(FPS);

    /***************************************************************************
     * Init stuff
     ****************************************************************************/
    Vector2 origin = {0.5, 0.5};
    sprite_init_static_with_origin(&lose, "resources/lose.png", WIDTH / 2, HEIGHT / 2 - 150, origin);
    sprite_init_static_with_origin(&continueS, "resources/continue.png", WIDTH / 2, HEIGHT / 2 + 150, origin);
    input_init(&input);

    init_game();

    input_set_mouse_center(&input, &mole.sprite);
    input_set_device(&input, INPUT_GAMEPAD);

    snd_intro = LoadSoundEx("resources/speech/roots.wav", 2.0f);
    snd_lost = LoadSoundEx("resources/speech/lost.wav", 2.0f);
    PlaySound(snd_intro);

    music = LoadMusicStream("resources/bgm/doom.ogg");
    SetMusicVolume(music, 0.3f);
    PlayMusicStream(music);

    fontBm = LoadFont("resources/fonts/eater/eater-number.fnt");

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(MainLoop, FPS, 1);
#else
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        MainLoop();
    }
#endif

    /***************************************************************************
     * Cleanup
     ****************************************************************************/
    CloseWindow();
    CloseConsole();

    return 0;
}
