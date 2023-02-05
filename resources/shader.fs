#version 330

// Input vertex attributes (from vertex shader)
in vec3 vertexPos;
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D t0;
uniform sampler2D texture_sand;

out vec4 finalColor;

void main()
{
    // Texel color fetching from texture sampler
    vec4 texelColor0 = texture(t0, fragTexCoord);
    vec4 texelColor1 = texture(texture_sand, fragTexCoord);

    // bool isSand = texelColor0.x == 237/255.0f
    //     && texelColor0.y == 214/255.0f
    //     && texelColor0.z == 92/255.0f;
    bool isSand = false;

    finalColor = isSand ? texelColor1 : texelColor0;
}