#version 330 core

in vec2 fragtexcoord;
uniform sampler2D fbtexture;

out vec4 finalfragcolor;

void main(void) {
	
	// vec3 w = vec3(0.2125, 0.7154, 0.0721);
	// vec3 irgb = texture(fbtexture, fragtexcoord).rgb;
	// float luminance = dot(irgb, w);
	// finalfragcolor = vec4(luminance, luminance, luminance, 1.) * vec4(irgb, 1.0) * 2 * vec4(0.7, 0.7, 1.3, 1.0);
	// finalfragcolor = texture(fbtexture, fragtexcoord) * vec4(0.2, 0.2, 1.0, 1.0);
	finalfragcolor = texture(fbtexture, fragtexcoord);
	
}
	

