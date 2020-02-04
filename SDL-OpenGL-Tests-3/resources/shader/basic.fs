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


struct LightSourceData {
    vec3 lightColor;
    vec3 lightPos;
};

struct DirectionalLightData {
    vec3 position;
    vec3 direction;
    vec3 lightColor;
    float cutOff;
    float outerCutOff;
};

uniform LightSourceData lights[100];
uniform int validObjects;
uniform int reflection;
uniform int useReflectionMap;
uniform sampler2D reflectionMap;


uniform DirectionalLightData data;

void main() {
    vec3 result = vec3(0.0f);
    
    
    vec3 normal = normalize(Normal);
    vec3 lightDir;
    float diff;
    vec3 diffuse;
    vec3 viewDir = normalize(viewPos - Vertex);
    vec3 reflectDir;
    
    
    int internalReflection;
    if(useReflectionMap == 0) {
        internalReflection = reflection;
    }
    else {
        vec3 refMap = texture(reflectionMap, UV).rgb;
        internalReflection = int((refMap.r + refMap.g + refMap.b) / 3.0f * 128);
    }
    
    vec3 specular;
    
    
    float dist;
    float attenuation;
    
    for(int i = 0; i < validObjects; i++) {
        lightDir = normalize(lights[i].lightPos - Vertex);
        diff = max(dot(normal, lightDir), 0.0f);
        diffuse = diff * lights[i].lightColor;
        
        reflectDir = reflect(-lightDir, normal);
        
        
        if(internalReflection == 0)
            specular = vec3(0.0f);
        else
            specular = 0.5f * pow(max(dot(viewDir, reflectDir), 0.0f), internalReflection) * lights[i].lightColor;
        
        
        dist = length(lights[i].lightPos - Vertex);
        attenuation = 1.0f / (1.0f + linear * dist + quadratic * (dist * dist));
        
        result += (diffuse + specular) * attenuation;
    }
    
    
    lightDir = normalize(data.position - Vertex);
    diff = max(dot(normal, lightDir), 0.0f);
    diffuse = diff * data.lightColor;
    
    reflectDir = reflect(-lightDir, normal);
    
    
    if(internalReflection == 0)
        specular = vec3(0.0f);
    else
        specular = 0.5f * pow(max(dot(viewDir, reflectDir), 0.0f), internalReflection) * data.lightColor;
    
    
    dist = length(data.position - Vertex);
    attenuation = 1.0f / (1.0f + linear * dist + quadratic * (dist * dist));
    
    result += (diffuse + specular) * attenuation * clamp((dot(lightDir, normalize(-data.direction)) - data.outerCutOff) / (data.cutOff - data.outerCutOff), 0.0f, 1.0f);
    
    
    color = vec4(ambientLight + result, 1.0f) * texture(tex, UV);
    color.rgb = pow(color.rgb, vec3(2.2f));
}
