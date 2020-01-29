#version 330 core

out vec4 color;

in vec3 Vertex;
in vec2 UV;
in vec3 Normal;


uniform sampler2D tex;
uniform vec3 viewPos;


const float linear = 0.09f;
const float quadratic = 0.032f;

const vec3 ambientLight = vec3(0.1f);


struct LightSourceData {
    vec3 lightColor;
    vec3 lightPos;
};

uniform LightSourceData lights[100];
uniform int validObjects;
uniform int reflection;
uniform int useReflectionMap;
uniform sampler2D reflectionMap;

void main() {
    vec3 result = vec3(0.0f);
    
    color = vec4(viewPos, 1.0f);
    
    for(int i = 0; i < validObjects; i++) {
        vec3 normal = normalize(Normal);
        
        vec3 lightDir = normalize(lights[i].lightPos - Vertex);
        vec3 diffuse = max(dot(normal, lightDir), 0.0f) * lights[i].lightColor;
        
        vec3 viewDir = normalize(viewPos - Vertex);
        vec3 reflectDir = reflect(-lightDir, normal);
        
        int ref;
        if(useReflectionMap == 0) {
            ref = reflection;
        }
        else {
            vec3 refMap = texture(reflectionMap, UV).xyz;
            ref = int((refMap.x + refMap.y + refMap.z) / 3.0f * 64);
            discard;
        }
        
        vec3 specular = 0.5f * pow(max(dot(viewDir, reflectDir), 0.0f), ref) * lights[i].lightColor;
        
        
        float dist = length(lights[i].lightPos - Vertex);
        float attenuation = 1.0f / (linear * dist + quadratic * (dist * dist));
        
        
        result += (diffuse + specular) * attenuation;
    }
    
    color = vec4(ambientLight + result, 1.0f) * texture(tex, UV);
    color.rgb = pow(color.rgb, vec3(2.2f));
}

