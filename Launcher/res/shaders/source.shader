#shader vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out VS_OUT
{
    vec3 fragPos;
    vec3 normal;
    vec2 texCoord;
} vs_out;

uniform mat4 uMVP;

void main()
{
    vs_out.fragPos = position;
    vs_out.normal = normal;
    vs_out.texCoord = texCoord;
    gl_Position = uMVP * vec4(position, 1.0);
}

#shader fragment
#version 430 core

out vec4 fragmentColor;

in VS_OUT
{
    vec3 fragPos;
    vec3 normal;
    vec2 texCoord;
} vs_in;

struct Material
{
    sampler2D texture_diffuse1;
};

uniform Material uMaterial;

void main()
{
    fragmentColor = texture(uMaterial.texture_diffuse1, vs_in.texCoord);
}