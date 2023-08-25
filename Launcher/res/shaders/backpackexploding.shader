#shader vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 uMVP;
uniform mat4 uModel;
uniform mat3 uTranInvModel;

out VS_OUT 
{
    vec2 texCoord;
    vec3 normal;
    vec3 fragPos;
} vs_out;

void main()
{
    vs_out.normal = uTranInvModel * normal;
    vs_out.fragPos = vec3(uModel * vec4(position, 1.0));
    vs_out.texCoord = texCoord;

    gl_Position = uMVP * vec4(position, 1.0);
}

#shader geometry
#version 430 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT 
{
    vec2 texCoord;
    vec3 normal;
    vec3 fragPos;
} gs_in[];

out GS_OUT
{
    vec2 texCoord;
    vec3 normal;
    vec3 fragPos;
} gs_out;

uniform float uTime;
uniform float uMagnitude;

vec3 GetNormal()
{
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a, b));
}

vec4 explode(vec4 position, vec3 normal)
{
    vec3 direction = normal * (sin(uTime) + 1.0) * uMagnitude;
    return position + vec4(direction, 0.0);
}

void main()
{
    vec3 normal = GetNormal();

    gl_Position = explode(gl_in[0].gl_Position, normal);
    gs_out.texCoord = gs_in[0].texCoord;
    gs_out.normal = gs_in[0].normal;
    gs_out.fragPos = gs_in[0].fragPos;
    EmitVertex();

    gl_Position = explode(gl_in[1].gl_Position, normal);
    gs_out.texCoord = gs_in[1].texCoord;
    gs_out.normal = gs_in[1].normal;
    gs_out.fragPos = gs_in[1].fragPos;
    EmitVertex();

    gl_Position = explode(gl_in[2].gl_Position, normal);
    gs_out.texCoord = gs_in[2].texCoord;
    gs_out.normal = gs_in[2].normal;
    gs_out.fragPos = gs_in[2].fragPos;
    EmitVertex();

    EndPrimitive();
}

#shader fragment
#version 430 core

out vec4 fragmentColor;

in GS_OUT 
{
    vec2 texCoord;
    vec3 normal;
    vec3 fragPos;
} fs_in;

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
    vec3 norm = normalize(fs_in.normal);
    vec3 viewDir = normalize(uViewPos - fs_in.fragPos);

    vec3 result = calcDirLight(uDirLight, norm, viewDir);

    result += calcPointLight(uPointLight, norm, fs_in.fragPos, viewDir);

    //result += calcSpotLight(uSpotLight, norm, fs_in.fragPos, viewDir);

    fragmentColor = vec4(result, 1.0);
}


vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    vec3 ambient = light.ambient * vec3(texture(uMaterial.texture_diffuse1, fs_in.texCoord));

    vec3 diffuse = light.diffuse * calcDiff(normal, lightDir) * vec3(texture(uMaterial.texture_diffuse1, fs_in.texCoord));

    vec3 specular = light.specular * calcSpec(normal, lightDir, viewDir) * vec3(texture(uMaterial.texture_specular1, fs_in.texCoord));

    return (ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    vec3 ambient = light.ambient * vec3(texture(uMaterial.texture_diffuse1, fs_in.texCoord));
    
    vec3 diffuse = light.diffuse * calcDiff(normal, lightDir) * vec3(texture(uMaterial.texture_diffuse1, fs_in.texCoord));

    vec3 specular = light.specular * calcSpec(normal, lightDir, viewDir) * vec3(texture(uMaterial.texture_specular1, fs_in.texCoord));

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

    vec3 ambient = light.ambient * vec3(texture(uMaterial.texture_diffuse1, fs_in.texCoord));

    vec3 diffuse = light.diffuse * calcDiff(normal, lightDir) * vec3(texture(uMaterial.texture_diffuse1, fs_in.texCoord));

    vec3 specular = light.specular * calcSpec(normal, lightDir, viewDir) * vec3(texture(uMaterial.texture_specular1, fs_in.texCoord));

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
    vec3 reflectDir = reflect(-lightDir, normal);
    return pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess);
}
