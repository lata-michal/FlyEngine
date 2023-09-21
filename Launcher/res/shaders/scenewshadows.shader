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

vec2 poissonDisk[16] = vec2[]( 
   vec2( -0.94201624, -0.39906216 ), 
   vec2( 0.94558609, -0.76890725 ), 
   vec2( -0.094184101, -0.92938870 ), 
   vec2( 0.34495938, 0.29387760 ), 
   vec2( -0.91588581, 0.45771432 ), 
   vec2( -0.81544232, -0.87912464 ), 
   vec2( -0.38277543, 0.27676845 ), 
   vec2( 0.97484398, 0.75648379 ), 
   vec2( 0.44323325, -0.97511554 ), 
   vec2( 0.53742981, -0.47373420 ), 
   vec2( -0.26496911, -0.41893023 ), 
   vec2( 0.79197514, 0.19090188 ), 
   vec2( -0.24188840, 0.99706507 ), 
   vec2( -0.81409955, 0.91437590 ), 
   vec2( 0.19984126, 0.78641367 ), 
   vec2( 0.14383161, -0.14100790 ) 
);

float random(vec3 seed, int i)
{
    float product = dot(vec4(seed, i), vec4(53.612, 12.123, 14.213, 93.955));
    return fract(sin(product) * 37373.2137);
}

void main()
{
    vec3 color = texture(uMaterial.texture_diffuse1, vs_in.oTexCoord).rgb;
    vec3 normal = normalize(vs_in.oNormal);
    vec3 lightColor = vec3(1.0);

    vec3 ambient = 0.3 * color;

    vec3 lightDir = normalize(uLightPos - vs_in.oFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    if(uLightPos.y < -0.5)
    {
        diff = 0.0;
    }
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

    float shadow = 0.0;
    float currentDepth = projCoord.z;
    float bias = max(0.05 * (1.0 - dot(normalize(vs_in.oNormal), normalize(uLightPos - vs_in.oFragPos))), 0.005);
    //vec2 texelSize = 1.0 / textureSize(uDepthMap, 0);

    //for(int x = -1; x <= 1; x++)
    //{
    //    for(int y = -1; y <= 1; y++)
    //    {
    //        float pcfDepth = texture(uDepthMap, projCoord.xy + vec2(x, y) * texelSize).r;
    //
    //        shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
    //    }
    //}
    //
    //shadow /= 9.0;

    for(int i = 0; i < 4; i++)
    {
        int index = int(16 * random(floor(vs_in.oFragPos.xyz * 1000.0), i));

        if(texture(uDepthMap, projCoord.xy + poissonDisk[index] / 1000.0).r < currentDepth - bias)
        {
            shadow += 0.25 * (texture(uDepthMap, projCoord.xy + poissonDisk[index] / 1000.0).r, currentDepth - bias);
        }
    }

    if(projCoord.z > 1.0)
    {
        shadow = 0.0;
    }

    return min(shadow, 1.0);
}