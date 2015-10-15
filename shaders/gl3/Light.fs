#version 150

uniform vec3 lcolor;

in vec2 texc;
in vec3 nrm;

out vec4 fragColor;

void main()
{
	fragColor = vec4(lcolor, 1.0);
}

