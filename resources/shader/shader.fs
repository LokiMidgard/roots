#version 330

// Input vertex attributes (from vertex shader)
in vec3 vertexPos;
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D t0;
uniform sampler2D texture_map;
uniform float scroll_position;

// out
out vec4 finalColor;

// helpers
#define WIDTH 960
#define HEIGHT 540
#define TILEMAP_WIDTH 256.0f
#define TILEMAP_HEIGHT 256.0f
#define TILE_WIDTH 32
#define TILE_HEIGHT 32
#define SCALE 3.0f

#define SF (1.0f/SCALE)
#define TM_WF (SF*WIDTH/TILEMAP_WIDTH)
#define TM_HF (SF*HEIGHT/TILEMAP_HEIGHT)
#define T_WM (TILE_WIDTH/TILEMAP_WIDTH)
#define T_HM (TILE_HEIGHT/TILEMAP_HEIGHT)

#define check(c,r,g,b) (c.x == r/255.0f && c.y == g/255.0f && c.z == b/255.0f)

void main()
{
    vec4 c0 = texture(t0, fragTexCoord);

    bool isSand = check(c0, 237, 214, 92);
    bool isEarth = check(c0, 127, 106, 79);
    bool isTunel = check(c0, 76, 63, 47);
    bool isStone = check(c0, 130, 130, 130);
    bool isEmerald = check(c0, 3, 228, 48);
    bool isQuickStone = check(c0, 3, 121, 241);
    bool isDigStone = check(c0, 230, 41, 55);

    vec2 index = isEarth ? vec2(1,0)
        : isSand ? vec2(2,0)
        : isTunel ? vec2(3,0)
        : isStone ? vec2(4,0)
        : isEmerald ? vec2(5,0)
        : isQuickStone ? vec2(6,0)
        : isDigStone ? vec2(7,0)
        : vec2(0,0);

    vec2 offset =  vec2(0, isTunel ? scroll_position * 0.5 : scroll_position);
    vec2 offsetCoord = vec2(fragTexCoord + offset);

    vec2 scale =  vec2(offsetCoord.x * TM_WF, offsetCoord.y * TM_HF);
    scale =  vec2(
        mod(scale.x, T_WM) + index.x * T_WM,
        mod(scale.y, T_HM) + index.y * T_HM
    );

    vec4 texelColor1 = texture(texture_map, scale);
    vec4 rootColor = vec4(1.0f/2.0f-(c0.z/2.0f),1.0f/4.0f-(c0.z/4.0f),1.0f/2.0f-(c0.z/2.0f),255.0f);

    bool isRoot = c0.x < 3/255.0f && c0.w > 0/255.0f;

    finalColor = isRoot ? rootColor : texelColor1;
}