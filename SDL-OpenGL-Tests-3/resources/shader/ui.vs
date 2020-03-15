#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec2 uv;

layout (std140) uniform uiProjView {
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

out vec2 UV;

uniform vec2 texOffset;

void main() {
    gl_Position = projection * view * model * vec4(vertex, 1.0f);
    UV = vec2(uv.x, 1.0f - uv.y) + vec2(texOffset.x, -texOffset.y);
}
