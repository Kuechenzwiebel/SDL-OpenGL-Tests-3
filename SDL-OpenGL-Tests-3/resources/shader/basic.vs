#version 330 core

layout (location = 0) in vec3 position;
//layout (location = 1) in vec4 colors;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//out vec4 Colors;

void main() {
    gl_Position = vec4(position, 1.0f);
//    Colors = colors;
}
