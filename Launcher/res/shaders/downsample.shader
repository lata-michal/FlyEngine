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

layout(location = 0) out vec3 downsample;

in vec2 oTexCoord;

uniform sampler2D uSourceTex;
uniform vec2 uSourceResolution;
uniform int uFirstIteration;
uniform float uApplyBloomThreshold;

void main()
{
    bool jmp = true;

    if(uFirstIteration == 1)
    {
        float brightness = dot(texture(uSourceTex, oTexCoord).rgb, vec3(0.2126, 0.7152, 0.0722));

        if(brightness <= uApplyBloomThreshold)
        {
            downsample = vec3(0.0);
            jmp = false;
        }
    }
    if(jmp == true)
    {
        vec2 sourceTexelSize = 1.0 / uSourceResolution;
        float texelX = sourceTexelSize.x;
        float texelY = sourceTexelSize.y;

        // a b c
        //  j k 
        // d e f
        // l m
        // g h i

        vec3 a = texture(uSourceTex, vec2(oTexCoord.x - 2 * texelX, oTexCoord.y + 2 * texelY)).rgb;
        vec3 b = texture(uSourceTex, vec2(oTexCoord.x, oTexCoord.y + 2 * texelY)).rgb;
        vec3 c = texture(uSourceTex, vec2(oTexCoord.x + 2 * texelX, oTexCoord.y + 2 * texelY)).rgb;

        vec3 d = texture(uSourceTex, vec2(oTexCoord.x - 2 * texelX, oTexCoord.y)).rgb;
        vec3 e = texture(uSourceTex, vec2(oTexCoord.x, oTexCoord.y)).rgb;
        vec3 f = texture(uSourceTex, vec2(oTexCoord.x + 2 * texelX, oTexCoord.y)).rgb;

        vec3 g = texture(uSourceTex, vec2(oTexCoord.x - 2 * texelX, oTexCoord.y - 2 * texelY)).rgb;
        vec3 h = texture(uSourceTex, vec2(oTexCoord.x, oTexCoord.y - 2 * texelY)).rgb;
        vec3 i = texture(uSourceTex, vec2(oTexCoord.x + 2 * texelX, oTexCoord.y - 2 * texelY)).rgb;

        vec3 j = texture(uSourceTex, vec2(oTexCoord.x - texelX, oTexCoord.y + texelY)).rgb;
        vec3 k = texture(uSourceTex, vec2(oTexCoord.x + texelX, oTexCoord.y + texelY)).rgb;
        vec3 l = texture(uSourceTex, vec2(oTexCoord.x - texelX, oTexCoord.y - texelY)).rgb;
        vec3 m = texture(uSourceTex, vec2(oTexCoord.x + texelX, oTexCoord.y - texelY)).rgb;

        downsample = e * 0.125;
        downsample += (a + c + g + i) * 0.03125;
        downsample += (b + d + f + h) * 0.0625;
        downsample += (j + k + l + m) * 0.125;
    }
}