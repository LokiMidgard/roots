#version 330

// Input vertex attributes (from vertex shader)
in vec3 vertexPos;
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D t0;
uniform sampler2D texture_sand;
uniform sampler2D texture_earth;

uniform float scroll_position;


out vec4 finalColor;

void main()
{
    vec2 offest =  vec2(0, scroll_position);

    // Texel color fetching from texture sampler
    vec4 texelColor0 = texture(t0, fragTexCoord);
    vec4 texelColor_sand = texture(texture_sand, (fragTexCoord+offest) * 2);
    vec4 texelColor_earth = texture(texture_earth, (fragTexCoord+offest) * 2);

    vec4 root = vec4(texelColor0.z/2,texelColor0.z/4,texelColor0.z/2,255.0f);

    bool isSand = texelColor0.x == 237/255.0f
        && texelColor0.y == 214/255.0f
        && texelColor0.z == 92/255.0f;

        
    bool isEarth = texelColor0.x == 127/255.0f
        && texelColor0.y == 106/255.0f
        && texelColor0.z == 79/255.0f;

    bool isRoot = texelColor0.x <= 3/255.0f
        && texelColor0.w > 0/255.0f;
    //  bool isEarth = true;

    finalColor = isRoot
        ? root
        : isSand 
        ? texelColor_sand 
        : isEarth
        ? texelColor_earth
        : texelColor0;
}