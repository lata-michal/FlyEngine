#shader vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out vec3 oNormal;
out vec3 oPosition;

uniform mat3 uTranInvModel;
uniform mat4 uMVP;
uniform mat4 uModel;

void main()
{
    oNormal = uTranInvModel * normal;
    oPosition = vec3(uModel * vec4(position, 1.0));
    gl_Position = uMVP * vec4(position, 1.0);
}

#shader fragment
#version 430 core

out vec4 fragmentColor;

in vec3 oNormal;
in vec3 oPosition;

uniform vec3 uCameraPos;
uniform samplerCube uSkybox;

void main()
{
    float ratio = 1.00 / 1.52;
    vec3 I = normalize(oPosition - uCameraPos);
    vec3 R = refract(I, normalize(oNormal), ratio);
    fragmentColor = vec4(texture(uSkybox, R).rgb , 1.0);
}