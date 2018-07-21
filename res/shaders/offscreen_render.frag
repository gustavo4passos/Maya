#version 330 core

in vec2 fragtexcoord;
uniform sampler2D fbtexture;

out vec4 finalfragcolor;

void main(void) {
	finalfragcolor = texture(fbtexture, fragtexcoord);
}
	

