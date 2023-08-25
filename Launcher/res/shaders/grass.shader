#shader vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in mat4 instanceMatrix;

out vec2 oTexCoord;
out vec3 oFragPos;

uniform mat4 uVP;

void main()
{
    oTexCoord = texCoord;
    gl_Position = uVP * instanceMatrix * vec4(position, 1.0);
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
   vec4 texColor = texture(uMaterial.texture_diffuse1, oTexCoord);
   if(texColor.a < 0.1)
        discard;
    fragmentColor = texColor;

}


