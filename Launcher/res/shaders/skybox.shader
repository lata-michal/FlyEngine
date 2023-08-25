#shader vertex
#version 430 core

layout(location = 0) in vec3 position;

out vec3 oTexCoord;

uniform mat4 uVP;

void main()
{
    oTexCoord = position;
    vec4 pos = uVP * vec4(position, 1.0);
    gl_Position = pos.xyww;
}

#shader fragment
#version 430 core

out vec4 fragmentColor;

in vec3 oTexCoord;

uniform samplerCube uSkybox;

void main()
{
   fragmentColor = texture(uSkybox, oTexCoord);
}


