#shader vertex
#version 430 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec3 color;

out VS_OUT {
    vec3 vcolor;
} vs_out;

void main()
{
    gl_Position = vec4(position.xy, 0.0, 1.0);
    vs_out.vcolor = color;
}

#shader geometry
#version 430 core

layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

in VS_OUT {
    vec3 vcolor;
} gs_in[]; // geometry shader acts on set of vertices as its input

out vec3 fColor;

void build_house(vec4 position) 
{
    fColor = gs_in[0].vcolor;
    gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(0.2, -0.2, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(-0.2, 0.2, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(0.2, 0.2, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(0.0, 0.4, 0.0, 0.0);
    fColor = vec3(1.0);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    build_house(gl_in[0].gl_Position);
}


#shader fragment
#version 430 core

out vec4 fragmentColor;

in vec3 fColor;

void main()
{
    fragmentColor = vec4(fColor, 1.0);
}