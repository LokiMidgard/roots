#version 330

// Input vertex attributes (from vertex shader)
in vec3 vertexPos;
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D t0;

out vec4 finalColor;

void main()
{
    // Texel color fetching from texture sampler
    vec4 texelColor0 = texture(t0, fragTexCoord);
    finalColor = texelColor0;
}