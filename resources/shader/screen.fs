#version 330

// Input vertex attributes (from vertex shader)

in vec3 vertexPos;
varying vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D t0;

// out
out vec4 finalColor;

// helpers
#define WIDTH 960
#define HEIGHT 540

void main()
{
    vec4 c0 = texture(t0, fragTexCoord);

    // attenuate top and bottom
    float att = fragTexCoord.y < 0.3f ? min(1.0f, fragTexCoord.y * 10.0f)
              : fragTexCoord.y > 0.7f ? min(1.0f, (1.0f - fragTexCoord.y) * 10.0f)
              : 1.0f;

    finalColor = vec4(c0.x * att, c0.y * att, c0.z * att, c0.w);
}
