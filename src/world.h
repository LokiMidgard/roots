#ifndef ROOTS_WORLD_H
#define ROOTS_WORLD_H

#include "raylib.h"
#include "mole.h"

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
    for (int TerrainType = 0;
         TerrainType < TerrainTypeSize;
         ++TerrainType)
    {
        if (color_are_equal(c, TerrainTypeToColor[TerrainType]))
            return(TerrainType);
    }
    return(TerrainTypeSize);
};

#define MAX_WORMS 20

typedef struct World
{
    float speed;
    float pos_remainder;
    int number_of_images;
    Image images[10];
    int number_of_bg;
    Sprite bg[10];
    int number_of_fg;
    Sprite fg[10];
    Color* current_bitmap;
    Color* next_bitmap;
    Texture2D screen_texture;
    
    Shader shader;
    int shader_position_location;
    int shader_sand_location;
    Texture2D sand_texture;
    int shader_earth_location;
    Texture2D earth_texture;

    int number_of_active_worms;
    Vector2 worm_positions[MAX_WORMS];
    Texture2D worm_texture;
    
    int current_scroll;
    int depth;
    int leftSpeed;
    int centerSpeed;
    int rightSpeed;
} World;

typedef struct Dig
{
    int types[TerrainTypeSize];
} Dig;

void world_init(World *world);
void world_update(World *world, Mole *mole);
void world_draw(World *world);

#endif
