#shader vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out VS_OUT
{
    vec3 oFragPos;
    vec3 oNormal;
    vec2 oTexCoord;
} vs_out;

uniform mat4 uMVP;
uniform mat4 uModel;
uniform mat3 uTranInvModel;

void main()
{
    vs_out.oFragPos = vec3(uModel * vec4(position, 1.0));
    vs_out.oNormal = uTranInvModel * normal;
    vs_out.oTexCoord = texCoord;
    gl_Position = uMVP * vec4(position, 1.0);
}

#shader fragment
#version 430 core

out vec4 fragmentColor;

in VS_OUT
{
    vec3 oFragPos;
    vec3 oNormal;
    vec2 oTexCoord;
} vs_in;

struct Material
{
    sampler2D texture_diffuse1;
    float shininess;
};

uniform Material uMaterial;
uniform samplerCube uDepthMap;

uniform vec3 uLightPos;
uniform vec3 uViewPos;
uniform float uFarPlane;

vec3 diskOffsetDirection[20] = vec3[](
    vec3( 1, 1, 1), vec3( 1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
    vec3( 1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
    vec3( 1, 1, 0), vec3( 1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
    vec3( 1, 0, 1), vec3(-1, 0, 1), vec3( 1, 0, -1), vec3(-1, 0, -1),
    vec3( 0, 1, 1), vec3( 0, -1, 1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float ShadowCalc(vec3 fragPos)
{
    vec3 fragToLight = fragPos - uLightPos;

    float currentDepth = length(fragToLight);
    float bias = 0.15;
    float shadow = 0.0;
    float viewDistance = length(fragPos - uViewPos);
    float diskRadius = (1.0 + (viewDistance / uFarPlane)) / 40.0;
    
    for(int i = 0; i < 20; i++)
    {
        float closestDepth = texture(uDepthMap, fragToLight + diskOffsetDirection[i] * diskRadius).r;
        closestDepth *= uFarPlane;

        shadow += (currentDepth - bias > closestDepth) ? 1.0 : 0.0;
    }

    return shadow / 20.0;
}

void main()
{
    vec3 color = texture(uMaterial.texture_diffuse1, vs_in.oTexCoord).rgb;
    vec3 normal = normalize(vs_in.oNormal);
    vec3 lightColor = vec3(0.3);

    vec3 ambient = 0.3 * color;

    vec3 lightDir = normalize(uLightPos - vs_in.oFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(uViewPos - vs_in.oFragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = 0.0;
    if(diff > 0.0)
    {
        spec = pow(max(dot(normal, halfwayDir), 0.0), uMaterial.shininess);
    }
    vec3 specular = spec * lightColor;

    float distance = length(uLightPos - vs_in.oFragPos);
    float constant = 1.0;
    float linear = 0.05;
    float quadratic = 0.0005;
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

    float shadow = ShadowCalc(vs_in.oFragPos);

    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular) * attenuation) * color; 

    fragmentColor = vec4(lighting, 1.0);
}
