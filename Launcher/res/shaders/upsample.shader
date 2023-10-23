#shader vertex
#version 430 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;

out vec2 oTexCoord;

void main()
{
    gl_Position = vec4(position.x, position.y, 0.0, 1.0);
    oTexCoord = texCoord;
}


#shader fragment
#version 430 core

layout(location = 0) out vec3 upsample;

in vec2 oTexCoord;

uniform sampler2D uSourceTex;
uniform float uFilterRadius;

void main()
{
    float r = uFilterRadius;

    vec3 a = texture(uSourceTex, vec2(oTexCoord.x - r, oTexCoord.y + r)).rgb;
    vec3 b = texture(uSourceTex, vec2(oTexCoord.x, oTexCoord.y + r)).rgb;
    vec3 c = texture(uSourceTex, vec2(oTexCoord.x + r, oTexCoord.y + r)).rgb;

    vec3 d = texture(uSourceTex, vec2(oTexCoord.x - r, oTexCoord.y)).rgb;
    vec3 e = texture(uSourceTex, vec2(oTexCoord.x, oTexCoord.y)).rgb;
    vec3 f = texture(uSourceTex, vec2(oTexCoord.x + r, oTexCoord.y)).rgb;

    vec3 g = texture(uSourceTex, vec2(oTexCoord.x - r, oTexCoord.y - r)).rgb;
    vec3 h = texture(uSourceTex, vec2(oTexCoord.x, oTexCoord.y - r)).rgb;
    vec3 i = texture(uSourceTex, vec2(oTexCoord.x + r, oTexCoord.y - r)).rgb;

    upsample = 4.0 * e;
    upsample += 2.0 * (b + d + f + h);
    upsample += (a + c + g + i);
    upsample *= 1.0 / 16.0;
}