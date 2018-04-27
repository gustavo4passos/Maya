#version 400 core

layout(location = 2) in vec4 pos;
uniform out_pos;

void main() {
  gl_Position = vec4(pos);
}
