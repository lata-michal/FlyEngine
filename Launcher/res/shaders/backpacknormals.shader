#shader vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out VS_OUT
{
    vec3 normal;
} vs_out;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat3 uTranInvModel;

void main()
{
    vs_out.normal = uTranInvModel * normal;
    gl_Position = uView * uModel * vec4(position, 1.0);
}

#shader geometry
#version 430 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT
{
    vec3 normal;
} gs_in[];

const float MAGNITUDE = 0.2;

uniform mat4 uProjection;

void GenerateLine(int index)
{
    gl_Position = uProjection * gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = uProjection * (gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * MAGNITUDE);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    GenerateLine(0);
    GenerateLine(1);
    GenerateLine(2);
}

#shader fragment
#version 430 core

out vec4 fragmentColor;

void main()
{
    fragmentColor = vec4(1.0, 1.0, 0.0, 1.0);
}
