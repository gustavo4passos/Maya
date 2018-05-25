#version 330 core

in vec2 pos;
in vec3 color;
out vec3 outColor;

void main(void) {
    gl_Position = vec4(pos, 0.0, 1.0);
    outColor = color;
}