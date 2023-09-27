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

#shader geometry
#version 430 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT 
{
    vec3 fragPos;
    vec2 texCoord;
    vec3 tangentViewPos;
    vec3 tangentFragPos;
    vec3 tangentDirLightDirection;
    vec3 tangentPointLightPos;
    vec3 tangentSpotLightPos;
} vs_in[];

out GS_OUT
{
    vec3 fragPos;
    vec2 texCoord;
    vec3 tangentViewPos;
    vec3 tangentFragPos;
    vec3 tangentDirLightDirection;
    vec3 tangentPointLightPos;
    vec3 tangentSpotLightPos;
} gs_out;

uniform float uTime;
uniform float uMagnitude;

vec3 GetNormal()
{
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a, b));
}

vec4 implode(vec4 position, vec3 normal)
{
    vec3 direction = normal * (sin(uTime) + 1.0) * uMagnitude;
    return position - vec4(direction, 0.0);
}

void main()
{
    vec3 normal = GetNormal();

    gl_Position = implode(gl_in[0].gl_Position, normal);
    gs_out.texCoord = vs_in[0].texCoord;
    gs_out.fragPos = vs_in[0].fragPos;
    gs_out.tangentViewPos = vs_in[0].tangentViewPos;
    gs_out.tangentFragPos = vs_in[0].tangentFragPos;
    gs_out.tangentDirLightDirection = vs_in[0].tangentDirLightDirection;
    gs_out.tangentPointLightPos = vs_in[0].tangentPointLightPos;
    gs_out.tangentSpotLightPos = vs_in[0].tangentSpotLightPos;
    EmitVertex();

    gl_Position = implode(gl_in[1].gl_Position, normal);
    gs_out.texCoord = vs_in[1].texCoord;
    gs_out.fragPos = vs_in[1].fragPos;
    gs_out.tangentViewPos = vs_in[1].tangentViewPos;
    gs_out.tangentFragPos = vs_in[1].tangentFragPos;
    gs_out.tangentDirLightDirection = vs_in[1].tangentDirLightDirection;
    gs_out.tangentPointLightPos = vs_in[1].tangentPointLightPos;
    gs_out.tangentSpotLightPos = vs_in[1].tangentSpotLightPos;
    EmitVertex();

    gl_Position = implode(gl_in[2].gl_Position, normal);
    gs_out.texCoord = vs_in[2].texCoord;
    gs_out.fragPos = vs_in[2].fragPos;
    gs_out.tangentViewPos = vs_in[2].tangentViewPos;
    gs_out.tangentFragPos = vs_in[2].tangentFragPos;
    gs_out.tangentDirLightDirection = vs_in[2].tangentDirLightDirection;
    gs_out.tangentPointLightPos = vs_in[2].tangentPointLightPos;
    gs_out.tangentSpotLightPos = vs_in[2].tangentSpotLightPos;
    EmitVertex();

    EndPrimitive();
}

#shader fragment
#version 430 core

out vec4 fragmentColor;

in GS_OUT
{ 
    vec3 fragPos;
    vec2 texCoord;
    vec3 tangentViewPos;
    vec3 tangentFragPos;
    vec3 tangentDirLightDirection;
    vec3 tangentPointLightPos;
    vec3 tangentSpotLightPos;
    vec3 tangentSpotLightDirection;
} gs_in;

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
    vec3 norm = texture(uMaterial.texture_normal1, gs_in.texCoord).rgb;
    norm = normalize(norm * 2.0 - 1.0);

    vec3 viewDir = normalize(gs_in.tangentViewPos - gs_in.tangentFragPos);

    vec3 result = calcDirLight(uDirLight, norm, viewDir);

    //result += calcPointLight(uPointLight, norm, gs_in.fragPos, viewDir);

    result += calcSpotLight(uSpotLight, norm, gs_in.fragPos, viewDir);

    fragmentColor = vec4(result, 1.0);
}


vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-gs_in.tangentDirLightDirection);

    float diff = calcDiff(normal, lightDir);
    float spec = 0.0;
    if(diff > 0.0)
        spec = calcSpec(normal, lightDir, viewDir);

    vec3 ambient = light.ambient * vec3(texture(uMaterial.texture_diffuse1, gs_in.texCoord));

    vec3 diffuse = light.diffuse * diff * vec3(texture(uMaterial.texture_diffuse1, gs_in.texCoord));

    vec3 specular = light.specular * spec * vec3(texture(uMaterial.texture_specular1, gs_in.texCoord));

    return (ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(gs_in.tangentPointLightPos - gs_in.tangentFragPos);

    float diff = calcDiff(normal, lightDir);
    float spec = 0.0;
    if(diff > 0.0)
        spec = calcSpec(normal, lightDir, viewDir);

    vec3 ambient = light.ambient * vec3(texture(uMaterial.texture_diffuse1, gs_in.texCoord));

    vec3 diffuse = light.diffuse * diff * vec3(texture(uMaterial.texture_diffuse1, gs_in.texCoord));

    vec3 specular = light.specular * spec * vec3(texture(uMaterial.texture_specular1, gs_in.texCoord));

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient *= attenuation; 
    diffuse *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular);
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(gs_in.tangentSpotLightPos - gs_in.tangentFragPos);

    float diff = calcDiff(normal, lightDir);
    float spec = 0.0;
    if(diff > 0.0)
        spec = calcSpec(normal, lightDir, viewDir);

    vec3 ambient = light.ambient * vec3(texture(uMaterial.texture_diffuse1, gs_in.texCoord));

    vec3 diffuse = light.diffuse * diff * vec3(texture(uMaterial.texture_diffuse1, gs_in.texCoord));

    vec3 specular = light.specular * spec * vec3(texture(uMaterial.texture_specular1, gs_in.texCoord));

    //spotlight
    float theta = dot(normalize(light.position - fragPos), normalize(-light.direction));
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp(((theta - light.outerCutOff) / epsilon), 0.0, 1.0);
    
    diffuse *= intensity;
    specular *= intensity;

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 /  (light.constant + light.linear * distance + light.quadratic * (distance * distance));

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