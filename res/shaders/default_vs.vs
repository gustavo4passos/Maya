#version 400 core

layout(location = 2) in vec4 pos;
uniform vec4 out_pos;

void main() {
  gl_Position = vec4(pos);
}
