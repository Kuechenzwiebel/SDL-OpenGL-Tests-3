#version 330 core

out vec4 color;

in vec3 Vertex;
in vec2 UV;
in vec3 Normal;

uniform sampler2D tex;
uniform vec3 viewPos;

vec3 ambientLight = vec3(0.2f);
vec3 lightColor = vec3(0.0f, 0.8f, 0.5f);
vec3 lightPos = vec3(4.0f, 4.0f, 4.0f);

void main() {
    vec3 normal = normalize(Normal);
    
    vec3 lightDir = normalize(lightPos - Vertex);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    float specularStrength = 1.0f;
    vec3 viewDir = normalize(viewPos - Vertex);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 40);
    vec3 specular = specularStrength * spec * lightColor;
    
    color = vec4(ambientLight + diffuse + specular, 1.0f) * texture(tex, UV);
}

