#shader vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 uModel;

void main()
{
    gl_Position = uModel * vec4(position, 1.0);
}

#shader geometry
#version 430 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

uniform mat4 uShadowTransform[6];

out vec4 oFragPos;

void main()
{
    for(int face = 0; face < 6; face++)
    {
        gl_Layer = face;
        for(int i = 0; i < 3; i++)
        {
            oFragPos = gl_in[i].gl_Position;
            gl_Position = uShadowTransform[face] * oFragPos;
            EmitVertex();
        }
        EndPrimitive();
    }
}

#shader fragment
#version 430 core

in vec4 oFragPos;

uniform vec3 uLightPos;
uniform float uFarPlane;

void main()
{
    float lightDist = length(oFragPos.xyz - uLightPos);
    lightDist = lightDist / uFarPlane;
    
    gl_FragDepth = lightDist;
}