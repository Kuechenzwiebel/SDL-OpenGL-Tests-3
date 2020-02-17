#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec4 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 col;

void main() {
    gl_Position = projection * view * vec4(vertex, 1.0f);
    col = color;
}

