#version 330 core

in vec2 outtexcoord;
out vec4 finalcolor;

uniform sampler2D texdata;

void main(void){

    vec4 test  = texture(texdata, outtexcoord);
    finalcolor = test;
}
