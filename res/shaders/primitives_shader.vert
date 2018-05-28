#version 330 core

layout(location = 0) in vec2 pos;

uniform mat4 ortho;
uniform mat4 model;

void main(void) {

     gl_Position = ortho * model * vec4(pos, 0.0, 1.0);
}