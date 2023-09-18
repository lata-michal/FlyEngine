#shader vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec3 oNormal;
out vec2 oTexCoord;
out vec3 oFragPos;

uniform mat4 uMVP;
uniform mat4 uModel;
uniform mat3 uTranInvModel;

void main()
{
    oNormal = uTranInvModel * normal;
    oTexCoord = texCoord;
    oFragPos = vec3(uModel * vec4(position, 1.0));
    gl_Position = uMVP * vec4(position, 1.0);
}

#shader fragment
#version 430 core

out vec4 fragmentColor;

in vec3 oNormal;
in vec2 oTexCoord;
in vec3 oFragPos;

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;

    float shininess;
};

struct DirLight
{
    vec3 direction;

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
uniform vec3 uViewPos;

void main()
{
    vec3 norm = normalize(oNormal);
    vec3 viewDir = normalize(uViewPos - oFragPos);

    vec3 result = calcDirLight(uDirLight, norm, viewDir);

    fragmentColor = vec4(result, 1.0);
}


vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    vec3 ambient = light.ambient * vec3(texture(uMaterial.texture_diffuse1, oTexCoord));

    vec3 diffuse = light.diffuse * calcDiff(normal, lightDir) * vec3(texture(uMaterial.texture_diffuse1, oTexCoord));
    
    vec3 specular = light.specular * calcSpec(normal, lightDir, viewDir) * vec3(texture(uMaterial.texture_specular1, oTexCoord));

    return (ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    vec3 ambient = light.ambient * vec3(texture(uMaterial.texture_diffuse1, oTexCoord));
    
    vec3 diffuse = light.diffuse * calcDiff(normal, lightDir) * vec3(texture(uMaterial.texture_diffuse1, oTexCoord));

    vec3 specular = light.specular * calcSpec(normal, lightDir, viewDir) * vec3(texture(uMaterial.texture_specular1, oTexCoord));

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient *= attenuation; 
    diffuse *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular);
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    vec3 ambient = light.ambient * vec3(texture(uMaterial.texture_diffuse1, oTexCoord));

    vec3 diffuse = light.diffuse * calcDiff(normal, lightDir) * vec3(texture(uMaterial.texture_diffuse1, oTexCoord));

    vec3 specular = light.specular * calcSpec(normal, lightDir, viewDir) * vec3(texture(uMaterial.texture_specular1, oTexCoord));

    //spotlight
    float theta = dot(lightDir, normalize(-light.direction));
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
