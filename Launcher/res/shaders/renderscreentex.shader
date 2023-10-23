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

uniform sampler2D uSceneTex;
uniform sampler2D uBloomTex;

uniform float uExposure;

void main()
{
    vec3 hdrColor = texture(uSceneTex, oTexCoord).rgb;
    vec3 bloomColor = texture(uBloomTex, oTexCoord).rgb;

    hdrColor += bloomColor;
    vec3 result = vec3(1.0) - exp(-hdrColor * uExposure);

    fragmentColor = vec4(result, 1.0);
}


