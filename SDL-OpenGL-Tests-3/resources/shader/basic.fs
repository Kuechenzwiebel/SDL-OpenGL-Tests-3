#version 330 core

out vec4 color;

in vec3 Vertex;
in vec2 UV;
in vec3 Normal;

uniform sampler2D tex;
uniform vec3 viewPos;


vec3 ambientLight = vec3(0.2f);

struct LightSourceData {
    vec3 lightColor;
    vec3 lightPos;
    float specularStrength;
    
    int reflection;
};

uniform LightSourceData lights[100];
uniform int validObjects;

float constant = 1.0f;
float linear = 0.09f;
float quadratic = 0.032f;

void main() {
    vec3 result = vec3(0.0f);
    
    
    color = vec4(viewPos, 1.0f);
    
    for(int i = 0; i < validObjects; i++) {
        vec3 normal = normalize(Normal);
        
        vec3 lightDir = normalize(lights[i].lightPos - Vertex);
        vec3 diffuse = max(dot(normal, lightDir), 0.0) * lights[i].lightColor;
        
        vec3 viewDir = normalize(viewPos - Vertex);
        vec3 reflectDir = reflect(-lightDir, normal);
        vec3 specular = lights[i].specularStrength * pow(max(dot(viewDir, reflectDir), 0.0), lights[i].reflection) * lights[i].lightColor;
        
        
        float dist = length(lights[i].lightPos - Vertex);
        float attenuation = 1.0 / (constant + linear * dist +
                        quadratic * (dist * dist));
        
        result += (diffuse + specular) * attenuation;
    }
    
    
    color = vec4(ambientLight + result, 1.0f) * texture(tex, UV);
}

