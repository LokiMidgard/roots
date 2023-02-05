#version 330

// Input vertex attributes (from vertex shader)
in vec3 vertexPos;
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D t0;

// out
out vec4 finalColor;

// helpers
#define WIDTH 960
#define HEIGHT 540
#define HW (WIDTH * 0.5f)
#define HH (HEIGHT * 0.5f)
#define check(c,r,g,b) (c.x == r/255.0f && c.y == g/255.0f && c.z == b/255.0f)

void main()
{
    vec4 c0 = texture(t0, fragTexCoord);
    float att = min(1.0f, fragTexCoord.y * 6.0f);
    finalColor = vec4(c0.x * att, c0.y * att, c0.z * att, c0.w);
}
