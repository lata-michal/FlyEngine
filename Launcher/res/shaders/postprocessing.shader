#shader vertex
#version 430 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;

out vec2 oTexCoord;

void main()
{
    oTexCoord = texCoord;
    gl_Position = vec4(position.x, position.y, 0.0, 1.0);
}

#shader fragment
#version 430 core

out vec4 fragmentColor;

in vec2 oTexCoord;

uniform sampler2D uScreenTexture;
uniform float uOffsetX;
uniform float uOffsetY;
uniform float uKernel[9];

void main()
{
    vec2 offsets[9] = vec2[] (
        vec2(-uOffsetX, uOffsetY),
        vec2(0.0, uOffsetY),
        vec2(uOffsetX, uOffsetY),
        vec2(-uOffsetX, 0.0),
        vec2(0.0, 0.0),
        vec2(uOffsetX, 0.0),
        vec2(-uOffsetX -uOffsetY),
        vec2(0.0, -uOffsetY),
        vec2(uOffsetX, -uOffsetY)
    );

    vec3 color = vec3(0.0);
    for(int i = 0; i < 9; i++)
        color += vec3(texture(uScreenTexture, oTexCoord.st + offsets[i])) * uKernel[i];
    
    fragmentColor = vec4(color, 1.0);

    //fragmentColor = texture(uScreenTexture, oTexCoord);
    //float average = 0.2126 * fragmentColor.r + 0.7152 * fragmentColor.b + 0.0722 * fragmentColor.b;
    //fragmentColor = vec4(average, average, average, 1.0);
}


