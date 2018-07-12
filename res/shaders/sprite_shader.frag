#version 330 core

in vec2 texcoorddata;
out vec4 fragcolor;

uniform sampler2D texdata;
uniform vec4 srcrct;

vec2 finalTex;

void main(void) {
    vec2 texDimensions = textureSize(texdata, 0);
    finalTex.x = texcoorddata.x * (srcrct.z / texDimensions.x) + (srcrct.x / texDimensions.x);
    finalTex.y = texcoorddata.y * (srcrct.w / texDimensions.y) + (srcrct.y / texDimensions.y);
   
    fragcolor = texture(texdata, finalTex);
}
