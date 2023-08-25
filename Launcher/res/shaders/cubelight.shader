#shader vertex
#version 430 core

layout(location = 0) in vec3 position;

uniform mat4 uMVP;

void main()
{
    gl_Position =  uMVP *  vec4(position, 1.0);
}

#shader fragment
#version 430 core

layout(location = 0) out vec4 fragmentColor;

void main()
{
    fragmentColor = vec4(1.0, 0.0, 0.0, 1.0);
}

