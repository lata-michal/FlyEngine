#shader vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

out vec2 oTexCoord;

void main()
{
    oTexCoord = texCoord;
    gl_Position = vec4(position, 1.0);
}

#shader fragment
#version 430 core

out vec4 fragColor;

in vec2 oTexCoord;

uniform sampler2D uDepthMap;

void main()
{
    float depthValue = texture(uDepthMap, oTexCoord).r;
    fragColor = vec4(vec3(depthValue), 1.0);
}

