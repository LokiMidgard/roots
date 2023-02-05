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
    BROWN,
    DARKBROWN,
    GRAY,
    CLITERAL(Color) {0, 0, 0, 20},
    CLITERAL(Color) {0, 1, 255, 255},
    CLITERAL(Color) {0, 255, 1, 255},
    GREEN,
    BLUE,
    RED,
    CLITERAL(Color) {237, 214, 92, 255},
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

typedef struct World
{
    float speed;
    float pos_remainder;
    int number_of_images;
    Image images[10];
    int number_of_bg;
    Sprite bg[10];
    Color* current_bitmap;
    Color* next_bitmap;
    Texture2D screen_texture;
    
    Shader shader;
    int texLoc;
    Texture2D sand_texture;
    
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
