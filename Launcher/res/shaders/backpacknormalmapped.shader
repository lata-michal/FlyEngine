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
    vec3 tangentViewPos;
    vec3 tangentFragPos;
    vec3 tangentDirLightDirection;
    vec3 tangentPointLightPos;
    vec3 tangentSpotLightPos;
} vs_out;

uniform vec3 uDirLightDirection;
uniform vec3 uPointLightPos;
uniform vec3 uSpotLightPos;
uniform vec3 uViewPos;

uniform mat4 uMVP;
uniform mat4 uModel;
uniform mat3 uTranInvModel;

void main()
{
    vs_out.fragPos = vec3(uModel * vec4(position, 1.0));
    vs_out.texCoord = texCoord;

    vec3 T = normalize(uTranInvModel * tangent);
    vec3 N = normalize(uTranInvModel * normal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    mat3 TBNmatrix = transpose(mat3(T, B, N));

    vs_out.tangentViewPos = TBNmatrix * uViewPos;
    vs_out.tangentFragPos = TBNmatrix * vs_out.fragPos;
    vs_out.tangentDirLightDirection = TBNmatrix * uDirLightDirection;
    vs_out.tangentPointLightPos = TBNmatrix * uPointLightPos;
    vs_out.tangentSpotLightPos = TBNmatrix * uSpotLightPos;

    gl_Position = uMVP * vec4(position, 1.0);
}

#shader fragment
#version 430 core

out vec4 fragmentColor;

in VS_OUT
{ 
    vec3 fragPos;
    vec2 texCoord;
    vec3 tangentViewPos;
    vec3 tangentFragPos;
    vec3 tangentDirLightDirection;
    vec3 tangentPointLightPos;
    vec3 tangentSpotLightPos;
    vec3 tangentSpotLightDirection;
} vs_in;

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_normal1;

    float shininess;
};

struct DirLight
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;

    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

float calcDiff(vec3 normal, vec3 lightDir);
float calcSpec(vec3 normal, vec3 lightDir, vec3 viewDir);

uniform Material uMaterial;
uniform DirLight uDirLight;
uniform PointLight uPointLight;
uniform SpotLight uSpotLight;

void main()
{
    vec3 norm = texture(uMaterial.texture_normal1, vs_in.texCoord).rgb;
    norm = normalize(norm * 2.0 - 1.0);

    vec3 viewDir = normalize(vs_in.tangentViewPos - vs_in.tangentFragPos);

    vec3 result = calcDirLight(uDirLight, norm, viewDir);

    result += calcPointLight(uPointLight, norm, vs_in.fragPos, viewDir);

    //result += calcSpotLight(uSpotLight, norm, vs_in.fragPos, viewDir);

    fragmentColor = vec4(result, 1.0);
}


vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-vs_in.tangentDirLightDirection);

    float diff = calcDiff(normal, lightDir);
    float spec = 0.0;
    if(diff > 0.0)
        spec = calcSpec(normal, lightDir, viewDir);

    vec3 ambient = light.ambient * vec3(texture(uMaterial.texture_diffuse1, vs_in.texCoord));

    vec3 diffuse = light.diffuse * diff * vec3(texture(uMaterial.texture_diffuse1, vs_in.texCoord));

    vec3 specular = light.specular * spec * vec3(texture(uMaterial.texture_specular1, vs_in.texCoord));

    return (ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(vs_in.tangentPointLightPos - vs_in.tangentFragPos);

    float diff = calcDiff(normal, lightDir);
    float spec = 0.0;
    if(diff > 0.0)
        spec = calcSpec(normal, lightDir, viewDir);

    vec3 ambient = light.ambient * vec3(texture(uMaterial.texture_diffuse1, vs_in.texCoord));

    vec3 diffuse = light.diffuse * diff * vec3(texture(uMaterial.texture_diffuse1, vs_in.texCoord));

    vec3 specular = light.specular * spec * vec3(texture(uMaterial.texture_specular1, vs_in.texCoord));

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient *= attenuation; 
    diffuse *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular);
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(vs_in.tangentSpotLightPos - vs_in.tangentFragPos);

    float diff = calcDiff(normal, lightDir);
    float spec = 0.0;
    if(diff > 0.0)
        spec = calcSpec(normal, lightDir, viewDir);

    vec3 ambient = light.ambient * vec3(texture(uMaterial.texture_diffuse1, vs_in.texCoord));

    vec3 diffuse = light.diffuse * diff * vec3(texture(uMaterial.texture_diffuse1, vs_in.texCoord));

    vec3 specular = light.specular * spec * vec3(texture(uMaterial.texture_specular1, vs_in.texCoord));

    //spotlight
    float theta = dot(normalize(light.position - fragPos), normalize(-light.direction));
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp(((theta - light.outerCutOff) / epsilon), 0.0, 1.0);
    
    diffuse *= intensity;
    specular *= intensity;

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

float calcDiff(vec3 normal, vec3 lightDir)
{
    return max(dot(normal, lightDir), 0.0);
}

float calcSpec(vec3 normal, vec3 lightDir, vec3 viewDir)
{
    //vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    return pow(max(dot(normal, halfwayDir), 0.0), uMaterial.shininess);
}
