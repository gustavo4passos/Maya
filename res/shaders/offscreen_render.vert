#version 330 core

layout(location = 0) in vec2 vpos;
layout(location = 1) in vec2 intexcoord;

out vec2 fragtexcoord;

void main(void) {
	gl_Position = vec4(vpos, 0.0, 1.0);
	fragtexcoord = intexcoord;

	float pixelsize = 1.0 / 1080.0;
}
