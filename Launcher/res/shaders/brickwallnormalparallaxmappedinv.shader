#shader vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

out VS_OUT
{
    vec3 fragPos;
    vec2 texCoord;
    vec3 tangentLightPos;
    vec3 tangentViewPos;
    vec3 tangentFragPos;
} vs_out;

uniform mat4 uMVP;
uniform mat4 uModel;
uniform mat3 uTranInvModel;

uniform vec3 uLightPos;
uniform vec3 uViewPos;

void main()
{
    vs_out.fragPos = vec3(uModel * vec4(position, 1.0));
    vs_out.texCoord = texCoord;

    vec3 T = normalize(uTranInvModel * tangent);
    vec3 N = normalize(uTranInvModel * normal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    mat3 TBNmatrix = transpose(mat3(T, B, N));
    
    vs_out.tangentLightPos = TBNmatrix * uLightPos;
    vs_out.tangentViewPos = TBNmatrix * uViewPos;
    vs_out.tangentFragPos = TBNmatrix * vs_out.fragPos;

    gl_Position = uMVP * vec4(position, 1.0);
}

#shader fragment
#version 430 core

out vec4 fragmentColor;

in VS_OUT
{
    vec3 fragPos;
    vec2 texCoord;
    vec3 tangentLightPos;
    vec3 tangentViewPos;
    vec3 tangentFragPos;
} vs_in;

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_normal1;
    sampler2D texture_displacement1;

    float shininess;
};

uniform Material uMaterial;
uniform float uHeightScale;

vec2 parallaxMapping(vec2 texCoordinate, vec3 viewDir)
{
    const float min = 8.0;
    const float max = 64.0;
    float numLayers = mix(max, min, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));

    float layerDepth = 1.0 / numLayers;
    float currentDepth = 0.0;

    vec2 P = viewDir.xy / viewDir.z * uHeightScale;
    vec2 deltaTexCoord = P / numLayers;

    vec2 currentTexCord = texCoordinate;
    float currentDispMapValue = 1.0 - texture(uMaterial.texture_displacement1, currentTexCord).r;

    while(currentDepth < currentDispMapValue)
    {
        currentDepth += layerDepth;
        currentTexCord -= deltaTexCoord;
        currentDispMapValue = 1.0 - texture(uMaterial.texture_displacement1, currentTexCord).r;
    }

    vec2 prevTexCoord = currentTexCord + deltaTexCoord;

    float beforeDepth = (1.0 - texture(uMaterial.texture_displacement1, prevTexCoord).r) - currentDepth + layerDepth;
    float afterDepth = currentDispMapValue - currentDepth;

    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoord * weight + currentTexCord * (1.0 - weight);

    return finalTexCoords;
}

void main()
{
    vec3 lightDir = normalize(vs_in.tangentLightPos - vs_in.tangentFragPos);
    vec3 viewDir = normalize(vs_in.tangentViewPos - vs_in.tangentFragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    vec2 texCoord = parallaxMapping(vs_in.texCoord, viewDir);
    if(texCoord.x > 1.0 || texCoord.y > 1.0 || texCoord.x < 0.0 || texCoord.y < 0.0)
        discard;

    vec3 normal =  texture(uMaterial.texture_normal1, texCoord).rgb;
    normal = normalize(normal * 2.0 - 1.0);

    vec3 color = texture(uMaterial.texture_diffuse1, texCoord).rgb;

    vec3 ambient = 0.01 * color;
    
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color * 0.5;

    float spec = 0.0;
    if(diff > 0.0)
        spec = pow(max(dot(halfwayDir, normal), 0.0), uMaterial.shininess);

    vec3 specular = vec3(0.1) * spec;

    fragmentColor = vec4(ambient + diffuse + specular, 1.0);
}

