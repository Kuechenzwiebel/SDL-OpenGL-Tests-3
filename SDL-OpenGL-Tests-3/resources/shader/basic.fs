#version 330 core

out vec4 color;

in vec3 Vertex;
in vec2 UV;
in vec3 Normal;


uniform sampler2D tex;
uniform vec3 viewPos;


const float linear = 0.07f;
const float quadratic = 0.017f;

const vec3 ambientLight = vec3(0.1f);


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

uniform PointLightSourceData pointLights[100];
uniform int validPointLights;

uniform SpotLightSourceData spotLights[100];
uniform int validSpotLights;

uniform int reflection;
uniform int useReflectionMap;
uniform sampler2D reflectionMap;


void main() {
    vec3 result = vec3(0.0f);
    
    
    vec3 normal = normalize(Normal);
    vec3 lightDir;
    float diff;
    vec3 diffuse;
    vec3 viewDir = normalize(viewPos - Vertex);
    vec3 reflectDir;
    
    vec3 specular;
    
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
    
    
    
    for(int i = 0; i < validPointLights; i++) {
        lightDir = normalize(pointLights[i].position - Vertex);
        diff = max(dot(normal, lightDir), 0.0f);
        diffuse = diff * pointLights[i].color;
        
        reflectDir = reflect(-lightDir, normal);
        
        
        if(internalReflection == 0)
            specular = vec3(0.0f);
        else
            specular = 0.5f * pow(max(dot(viewDir, reflectDir), 0.0f), internalReflection) * pointLights[i].color;
        
        
        dist = length(pointLights[i].position - Vertex);
        attenuation = 1.0f / (1.0f + linear * dist + quadratic * (dist * dist));
        
        result += (diffuse + specular) * attenuation;
    }
    
    for(int i = 0; i < validSpotLights; i++) {
        lightDir = normalize(spotLights[i].position - Vertex);
        diff = max(dot(normal, lightDir), 0.0f);
        diffuse = diff * spotLights[i].color;
        
        reflectDir = reflect(-lightDir, normal);
        
        
        if(internalReflection == 0)
            specular = vec3(0.0f);
        else
            specular = 0.5f * pow(max(dot(viewDir, reflectDir), 0.0f), internalReflection) * spotLights[i].color;
        
        
        dist = length(spotLights[i].position - Vertex);
        attenuation = 1.0f / (1.0f + linear * dist + quadratic * (dist * dist));
        
        result += (diffuse + specular) * attenuation * clamp((dot(lightDir, normalize(-spotLights[i].direction)) - spotLights[i].outerCutOff) / (spotLights[i].cutOff - spotLights[i].outerCutOff), 0.0f, 1.0f);
    }
    
    
    
    
    color = vec4(ambientLight + result, 1.0f) * texture(tex, UV);
    color.rgb = pow(color.rgb, vec3(2.2f));
}
