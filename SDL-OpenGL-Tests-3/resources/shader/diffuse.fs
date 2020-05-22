#version 330 core

out vec4 color;

in vec3 Vertex;
in vec2 UV;
in vec3 Normal;

layout (std140) uniform baseInformation {
    float time;
    float viewPosX;
    float viewPosY;
    float viewPosZ;
};

vec3 viewPos = vec3(viewPosX, viewPosY, viewPosZ);

uniform sampler2D tex;


const float linear = 0.07f;
const float quadratic = 0.017f;

const vec3 ambientLight = vec3(0.2f);


struct PointLightSourceData {
    vec3 color;
    vec3 position;
};

struct SpotLightSourceData {
    vec3 position;
    vec3 direction;
    vec3 color;
    float cutOff;
    float outerCutOff;
};

uniform PointLightSourceData pointLights[300];
uniform int validPointLights;

uniform SpotLightSourceData spotLights[30];
uniform int validSpotLights;

uniform int reflection;
uniform int useReflectionMap;
uniform sampler2D reflectionMap;

uniform int viewMode;

void main() {
    vec3 result = vec3(0.0f);
    
    
    vec3 forwardNormal = Normal;
    vec3 lightDir;
    float diffForward;
    vec3 diffuse;
    vec3 viewDir = normalize(viewPos - Vertex);
    
    float dist;
    float attenuation;
    
    
    int internalReflection;
    if(useReflectionMap == 0) {
        internalReflection = reflection;
    }
    else {
        vec3 refMap = texture(reflectionMap, UV).rgb;
        internalReflection = int((refMap.r + refMap.g + refMap.b) / 3.0f * 128);
    }
    
    
    lightDir = normalize(vec3(0.5f, 1.5f, 0.5f));
    diffForward = max(dot(forwardNormal, lightDir), 0.0f);
    diffuse = diffForward * vec3(0.17f);
    
    result += diffuse;
    
    
    for(int i = 0; i < validPointLights; i++) {
        dist = length(pointLights[i].position - Vertex);
        
        if(dist < 50.0f) {
            lightDir = normalize(pointLights[i].position - Vertex);
            diffForward = max(dot(forwardNormal, lightDir), 0.0f);
            diffuse = diffForward * pointLights[i].color;
            
            attenuation = 1.0f / (1.0f + linear * dist + quadratic * (dist * dist));
            
            result += diffuse * attenuation;
        }
    }
    
    for(int i = 0; i < validSpotLights; i++) {
        dist = length(spotLights[i].position - Vertex);
        
        if(dist < 50.0f) {
            lightDir = normalize(spotLights[i].position - Vertex);
            diffForward = max(dot(forwardNormal, lightDir), 0.0f);
            diffuse = diffForward * spotLights[i].color;
            
            attenuation = 1.0f / (1.0f + linear * dist + quadratic * (dist * dist));
            
            result += diffuse * attenuation * clamp((dot(lightDir, normalize(-spotLights[i].direction)) - spotLights[i].outerCutOff) / (spotLights[i].cutOff - spotLights[i].outerCutOff), 0.0f, 1.0f);
        }
    }
    
    
    if(viewMode == 1)
        color = vec4(forwardNormal, 1.0f);
    else if(viewMode == 2)
        color = vec4(normalize(UV), 0.0f, 1.0f);
    else
        color = vec4(ambientLight + result, 1.0f) * texture(tex, UV);
}
