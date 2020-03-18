#version 330 core

#include "resources/shader/shake.function"

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

layout (std140) uniform projView {
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

layout (std140) uniform baseInformation {
    float time;
    float viewPosX;
    float viewPosY;
    float viewPosZ;
};

vec3 viewPos = vec3(viewPosX, viewPosY, viewPosZ);

out vec3 Vertex;
out vec2 UV;
out vec3 Normal;

void main() {
    gl_Position = projection * view * model * vec4(vertex, 1.0f);
    Vertex = vec3(model * vec4(vertex, 1.0f));
    UV = vec2(uv.x, 1.0f - uv.y);
    Normal = normalize(mat3(transpose(inverse(model))) * normal);
    
    gl_Position.xy = shake(gl_Position.xy, time);
}
