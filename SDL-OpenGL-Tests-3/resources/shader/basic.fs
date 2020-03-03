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

uniform PointLightSourceData pointLights[300];
uniform int validPointLights;

uniform SpotLightSourceData spotLights[30];
uniform int validSpotLights;

uniform int reflection;
uniform int useReflectionMap;
uniform sampler2D reflectionMap;

uniform int normalView;

void main() {
    vec3 result = vec3(0.0f);
    
    
    vec3 forwardNormal = normalize(Normal);
    vec3 backwardNormal = -forwardNormal;
    vec3 lightDir;
    float diffForward, diffBackward;
    vec3 diffuse;
    vec3 viewDir = normalize(viewPos - Vertex);
    vec3 reflectDirForward, reflectDirBackward;
    
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
        dist = length(pointLights[i].position - Vertex);
        
        if(dist < 50.0f) {
            lightDir = normalize(pointLights[i].position - Vertex);
            diffForward = max(dot(forwardNormal, lightDir), 0.0f);
            diffBackward = max(dot(backwardNormal, lightDir), 0.0f);
            diffuse = (diffForward + diffBackward) * pointLights[i].color;
            
            reflectDirForward = reflect(-lightDir, forwardNormal);
            reflectDirBackward = reflect(-lightDir, backwardNormal);
            
            
            if(internalReflection == 0)
                specular = vec3(0.0f);
            else
                specular = 0.5f * (pow(max(dot(viewDir, reflectDirForward), 0.0f), internalReflection) + pow(max(dot(viewDir, reflectDirBackward), 0.0f), internalReflection)) * pointLights[i].color;
            
            
            attenuation = 1.0f / (1.0f + linear * dist + quadratic * (dist * dist));
            
            result += (diffuse + specular) * attenuation;
        }
    }
    
    for(int i = 0; i < validSpotLights; i++) {
        dist = length(spotLights[i].position - Vertex);
        
        if(dist < 50.0f) {
            lightDir = normalize(spotLights[i].position - Vertex);
            diffForward = max(dot(forwardNormal, lightDir), 0.0f);
            diffBackward = max(dot(backwardNormal, lightDir), 0.0f);
            diffuse = (diffForward + diffBackward) * spotLights[i].color;
            
            reflectDirForward = reflect(-lightDir, forwardNormal);
            
            
            if(internalReflection == 0)
                specular = vec3(0.0f);
            else
                specular = 0.5f * (pow(max(dot(viewDir, reflectDirForward), 0.0f), internalReflection) + pow(max(dot(viewDir, reflectDirBackward), 0.0f), internalReflection)) * spotLights[i].color;
            
            
            attenuation = 1.0f / (1.0f + linear * dist + quadratic * (dist * dist));
            
            result += (diffuse + specular) * attenuation * clamp((dot(lightDir, normalize(-spotLights[i].direction)) - spotLights[i].outerCutOff) / (spotLights[i].cutOff - spotLights[i].outerCutOff), 0.0f, 1.0f);
        }
    }
    
    
    if(normalView == 1)
        color = vec4(forwardNormal, 1.0f);
    else
        color = vec4(ambientLight + result, 1.0f) * texture(tex, UV);
}
