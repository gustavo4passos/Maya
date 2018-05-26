#version 330 core

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 texCoord;
out vec2 outTexCoord;

uniform mat4 model;
uniform mat4 ortho;

void main(void) {
    gl_Position = ortho * model *  vec4(pos, 0.0, 1.0);
    outTexCoord = texCoord;
}