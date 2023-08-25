#shader vertex
#version 430 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 offsets;

out vec3 fColor;

void main()
{
    vec2 spos = pos * (gl_InstanceID / 100.0);
    gl_Position = vec4(spos + offsets, 0.0, 1.0);
    fColor = color;
}

#shader fragment
#version 430 core

out vec4 fragmentColor;

in vec3 fColor;

void main()
{
    fragmentColor = vec4(fColor, 1.0);
}