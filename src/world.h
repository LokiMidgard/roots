#ifndef ROOTS_WORLD_H
#define ROOTS_WORLD_H

#include "raylib.h"
#include "config.h"
#include "mole.h"
#include "worms.h"

typedef enum TerrainType
{
    EARTH,
    TUNNEL,
    STONE,
    ROOT,
    ROOT_TIP,
    ROOT_KNOT,
    EMERALD,
    QUICK_STONE,
    DIG_STONE,
    SAND,
    TerrainTypeSize,
} TerrainType;

Color TerrainTypeToColor[TerrainTypeSize] =
{
    TERRA_EARTH,
    TERRA_TUNEL,
    TERRA_STONE,
    TERRA_ROOT,
    TERRA_ROOT_TIP,
    TERRA_ROOT_KNOT,
    TERRA_EMERALD,
    TERRA_QUICK_STONE,
    TERRA_DIG_STONE,
    TERRA_SAND,
};

Color
terrain_type_to_color(TerrainType t)
{
    return(TerrainTypeToColor[t]);
}

TerrainType
color_to_terrain_type(Color c)
{
    if(c.r<3 && c.a>0){ // roots have more colors 
        return ROOT;
    }
    for (int TerrainType = 0;
         TerrainType < TerrainTypeSize;
         ++TerrainType)
    {
        if (color_are_equal(c, TerrainTypeToColor[TerrainType]))
            return(TerrainType);
    }
    return(TerrainTypeSize);
};

typedef struct World
{
    // physics
    float speed;
    float pos_remainder;
    
    // graphics
    int number_of_images;
    Image images[10];
    int number_of_bg;
    Sprite bg[10];
    int number_of_fg;
    Sprite fg[10];

    Sprite title;

    // rendering
    int current_scroll;             // y-offset used for pixel lookups
    int last_scroll;                // number of lines scrolled in last frame
    int scrolling_paused;           // number of frames to stop scrolling
    Color* current_bitmap;
    Color* next_bitmap;
    Texture2D screen_texture;
    Shader shader;
    int shader_position_location;
    int shader_map_location;
    Texture2D map_texture;

    // Game Logic
    Worms worms;

    int depth;
} World;

typedef struct Dig
{
    int types[TerrainTypeSize];
} Dig;

void world_init(World *world);
void world_update(World *world);
void world_draw(World *world);

Dig world_check_dig(World *world, int x, int y, int radius);
Dig world_dig(World *world, int x, int y, int radius);

#endif
