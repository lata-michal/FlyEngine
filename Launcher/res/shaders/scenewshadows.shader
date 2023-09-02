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
    vec4 oFragPosLightSpace;
} vs_out;

uniform mat4 uModel;
uniform mat3 uTranInvModel;
uniform mat4 uMVP;
uniform mat4 uLightSpaceMatrix;

void main()
{
    vs_out.oFragPos = vec3(uModel * vec4(position, 1.0));
    vs_out.oNormal = uTranInvModel * normal;
    vs_out.oTexCoord = texCoord;
    vs_out.oFragPosLightSpace = uLightSpaceMatrix * vec4(vs_out.oFragPos, 1.0);
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
    vec4 oFragPosLightSpace;
} vs_in;

struct Material
{
    sampler2D texture_diffuse1;

    float shininess;
};

uniform Material uMaterial;
uniform sampler2D uDepthMap;

uniform vec3 uLightPos;
uniform vec3 uViewPos;

float calcShadow(vec4 fragPosLightSpace);

void main()
{
    vec3 color = texture(uMaterial.texture_diffuse1, vs_in.oTexCoord).rgb;
    vec3 normal = normalize(vs_in.oNormal);
    vec3 lightColor = vec3(1.0);

    vec3 ambient = 0.15 * lightColor;

    vec3 lightDir = normalize(uLightPos - vs_in.oFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(uViewPos - vs_in.oFragPos);
    vec3 halfwayDir = normalize(viewDir + lightDir);
    float spec = 0.0;
    if(diff > 0.0)
    {
        spec = pow(max(dot(normal, halfwayDir), 0.0), uMaterial.shininess);
    }
    vec3 specular = spec * lightColor;

    float shadow = calcShadow(vs_in.oFragPosLightSpace);

    vec3 light = (ambient + diffuse * (1.0 - shadow) + specular * (1.0 - shadow)) * color;
    fragmentColor = vec4(light, 1.0);
}

float calcShadow(vec4 fragPosLightSpace)
{
    vec3 projCoord = fragPosLightSpace.xyz / fragPosLightSpace.w;

    projCoord = 0.5 * projCoord + 0.5;

    float closestDepth = texture(uDepthMap, projCoord.xy).r;

    float currentDepth = projCoord.z;
    
    float bias = max(0.05 * (1.0 - dot(normalize(vs_in.oNormal), normalize(uLightPos - vs_in.oFragPos))), 0.005);
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    if(projCoord.z > 1.0)
    {
        shadow = 0.0;
    }

    return shadow;
}