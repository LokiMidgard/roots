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
#define PW (1.0f/WIDTH)
#define PH (1.0f/HEIGHT)
#define TM_WF (SF*WIDTH/TILEMAP_WIDTH)
#define TM_HF (SF*HEIGHT/TILEMAP_HEIGHT)
#define T_WM (TILE_WIDTH/TILEMAP_WIDTH)
#define T_HM (TILE_HEIGHT/TILEMAP_HEIGHT)

#define check(c,r,g,b) (c.x == r/255.0f && c.y == g/255.0f && c.z == b/255.0f)

#define isSand(c) check(c, 237, 214, 92)
#define isEarth(c) check(c, 127, 106, 79)
#define isTunel(c) check(c, 76, 63, 47)
#define isStone(c) check(c, 130, 130, 130)
#define isEmerald(c) check(c, 3, 228, 48)
#define isQuickStone(c) check(c, 3, 121, 241)
#define isDigStone(c) check(c, 230, 41, 55)
#define isRoot(c) (c.x < 3 / 255.f && c.w > 0)

void main()
{
    vec4 c0 = texture(t0, fragTexCoord);
    vec4 t = texture(t0, fragTexCoord + vec2(0, -PH));
    vec4 b = texture(t0, fragTexCoord + vec2(0, PH));

    vec2 offset =  vec2(0, isTunel(c0) ? scroll_position * 0.5 : scroll_position);
    vec2 offsetCoord = vec2(fragTexCoord + offset);

    vec2 index = isEarth(c0) ? vec2(1,0)
        : isSand(c0) ? vec2(2,0)
        : isTunel(c0) ? vec2(3,0)
        : isStone(c0) ? vec2(4,0)
        : isEmerald(c0) ? vec2(5,0)
        : isQuickStone(c0) ? vec2(6,0)
        : isDigStone(c0) ? vec2(7,0)
        : vec2(0,0);

    vec2 scale =  vec2(offsetCoord.x * TM_WF, offsetCoord.y * TM_HF);
    scale =  vec2(mod(scale.x, T_WM) + index.x * T_WM, mod(scale.y, T_HM) + index.y * T_HM);

    vec4 texelColor1 = texture(texture_map, scale);
    vec4 rootColor = vec4(1.0f/2.0f-(c0.z/2.0f),1.0f/4.0f-(c0.z/4.0f),1.0f/2.0f-(c0.z/2.0f),255.0f);

    finalColor = isRoot(c0) ? rootColor
        : !isTunel(c0) && isTunel(t) ? vec4(texelColor1.xyz * 2.0f, texelColor1.a)
        : !isTunel(c0) && isTunel(b) ? vec4(texelColor1.xyz * 0.4f, texelColor1.a)
        : texelColor1;
}
