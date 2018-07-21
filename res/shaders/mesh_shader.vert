#version 330 core

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 texcoord;
out vec2 outtexcoord;

uniform mat4 model;
uniform mat4 ortho;

void main(void) {
    outtexcoord = texcoord;
    gl_Position = ortho * model * vec4(pos, 0.f, 1.f);
}