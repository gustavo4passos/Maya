#version 330 core

in vec2 outTexCoord;
out vec4 finalColor;

uniform sampler2D texData;

void main(void) {
    finalColor = texture(texData, outTexCoord);
}