#shader vertex
#version 430 core

layout(location = 0) in vec3 position;

layout (std140) uniform uMatrices
{
    mat4 projection;
    mat4 view;
};

uniform mat4 uModel;

void main()
{
    gl_Position =  projection * view * uModel *  vec4(position.xyz, 1.0);
}

#shader fragment
#version 430 core

layout(location = 0) out vec4 fragmentColor;

void main()
{
    fragmentColor = vec4(0.0, 0.0, 1.0, 1.0);
}

