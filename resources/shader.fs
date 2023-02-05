#version 330

// Input vertex attributes (from vertex shader)
in vec3 vertexPos;
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D t0;
uniform sampler2D texture_map;

uniform float scroll_position;


out vec4 finalColor;

void main()
{

float scaleFactor = 3.0f;

vec4 texelColor0 = texture(t0, fragTexCoord);


    bool isSand = texelColor0.x == 237/255.0f
        && texelColor0.y == 214/255.0f
        && texelColor0.z == 92/255.0f;

        
    bool isEarth = texelColor0.x == 127/255.0f
        && texelColor0.y == 106/255.0f
        && texelColor0.z == 79/255.0f;
        
        
    bool isTunel = texelColor0.x == 76/255.0f
        && texelColor0.y == 63/255.0f
        && texelColor0.z == 47/255.0f;
    
    bool isStone = texelColor0.x == 130/255.0f
        && texelColor0.y == 130/255.0f
        && texelColor0.z == 130/255.0f;
    
    bool isEmerald = texelColor0.x == 3/255.0f
        && texelColor0.y == 228/255.0f
        && texelColor0.z == 48/255.0f;
    
    bool isQuickStone = texelColor0.x == 3/255.0f
        && texelColor0.y == 121/255.0f
        && texelColor0.z == 241/255.0f;
    
    bool isDigStone = texelColor0.x == 230/255.0f
        && texelColor0.y == 41/255.0f
        && texelColor0.z == 55/255.0f;

    //  bool isEarth = true;

    vec2 index = 
        isEarth
        ? vec2(1,0)
        : isSand 
        ? vec2(2,0)
    : isTunel 
        ? vec2(3,0)
        
        : isStone
        ? vec2(4,0)
        : isEmerald
        ? vec2(5,0)
        : isQuickStone
        ? vec2(6,0)
         : isDigStone
        ? vec2(7,0)
    //     ? texelColor_emerald
    //     ? texelColor_quick_stone
    //     ? texelColor_dig_stone
        
        : vec2(0,0);




    vec2 offest =  vec2(0, scroll_position);
    vec2 offestCoord = (fragTexCoord + offest);
    vec2 scale =  vec2(offestCoord.x*960.0f/ 256.0f *1.0f/scaleFactor ,offestCoord.y*540.0f/256.0f*1.0f/scaleFactor);
    vec2 scale2 =  vec2(mod(scale.x*256.0f,32.0f)/256.0f+index.x*32.0f/ 256.0f,mod(scale.y*256.0f,32.0f)/256.0f+index.y*32.0f/ 256.0f);
    
    



// vec2 lookup = mod((fragTexCoord + offest) , scale);

    // Texel color fetching from texture sampler
    
    

    vec4 texelColor1 = texture(texture_map, scale2);
    

    vec4 root = vec4(1.0f/2.0f-(texelColor0.z/2.0f),1.0f/4.0f-(texelColor0.z/4.0f),1.0f/2.0f-(texelColor0.z/2.0f),255.0f);

    bool isRoot = texelColor0.x < 3/255.0f
        && texelColor0.w > 0/255.0f;
    //  bool isEarth = true;

    finalColor = isRoot
        ? root     
        : texelColor1;
}