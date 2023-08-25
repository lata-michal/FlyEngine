#shader vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in mat4 instanceMatrix;

out vec2 oTexCoord;
out float oInstance;

uniform mat4 uVP;
uniform mat4 uMovement;
uniform mat4 uRotation;

void main()
{
    oTexCoord = texCoord;
    oInstance = gl_InstanceID;
    gl_Position = uVP * uMovement * instanceMatrix * uRotation * vec4(position, 1.0);
}

#shader fragment
#version 430 core

out vec4 fragmentColor;

in vec2 oTexCoord;
in float oInstance;

struct Material
{
    sampler2D texture_diffuse1;
};

uniform Material uMaterial;

void main()
{
    fragmentColor = texture(uMaterial.texture_diffuse1, oTexCoord) * vec4(0.9) * vec4((oInstance / 10000) * 0.7);
}


