#version 330 core

in vec2 outtexcoord;
out vec4 finalcolor;

uniform sampler2D texdata;

void main(void) {
	vec2 texdimensions = textureSize(texdata, 0);

	mat2 normaltextransform;
	normaltextransform[0] = vec2( 1.0 / texdimensions.x, 0.0);
	normaltextransform[1] = vec2( 0.0, 1.0 / texdimensions.y);
	vec2 normalizeduvcoords = normaltextransform * outtexcoord;
    finalcolor  = texture(texdata, normalizeduvcoords);
}
