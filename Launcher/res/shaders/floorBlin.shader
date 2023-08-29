#shader vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out VS_OUT
{
    vec3 fragPos;
    vec3 normal;
    vec2 texCoord;
} vs_out;

uniform mat4 uMVP;

void main()
{
    vs_out.fragPos = position;
    vs_out.normal = normal;
    vs_out.texCoord = texCoord;
    gl_Position = uMVP * vec4(position, 1.0);
}

#shader fragment
#version 430 core

out vec4 fragmentColor;

in VS_OUT
{
    vec3 fragPos;
    vec3 normal;
    vec2 texCoord;
} vs_in;

struct Material
{
    sampler2D texture_diffuse1;

    float shininess;
};

uniform vec3 uLightPos;
uniform vec3 uViewPos;
uniform Material uMaterial;

void main()
{
    vec3 color = texture(uMaterial.texture_diffuse1, vs_in.texCoord).rgb;

    vec3 ambient = 0.05 * color;

    vec3 lightDir = normalize(uLightPos - vs_in.fragPos);
    vec3 normal = normalize(vs_in.normal);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;

    float spec = 0.0;
    if(diff != 0.0)
    {
        vec3 viewDir = normalize(uViewPos - vs_in.fragPos);
        vec3 halfwayDir = normalize(viewDir + lightDir);
        spec = pow(max(dot(halfwayDir, vs_in.normal), 0.0), uMaterial.shininess);
    }
    vec3 specular = spec * vec3(0.3);

    fragmentColor = vec4(ambient + diffuse + specular, 1.0);
}