#shader vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

out vec2 oTexCoord;
out vec3 oFragPos;

uniform mat4 uMVP;

void main()
{
    oTexCoord = texCoord;
    gl_Position = uMVP * vec4(position, 1.0);
}

#shader fragment
#version 430 core

out vec4 fragmentColor;

in vec2 oTexCoord;

struct Material
{
    sampler2D texture_diffuse1;
};

uniform Material uMaterial;

void main()
{
  fragmentColor = texture(uMaterial.texture_diffuse1, oTexCoord);
}


