#version 150

in vec2 texc;
in vec3 nrm;

out vec4 fragColor;

void main()
{
	vec3 col = nrm * 0.5 + 0.5;
	fragColor = vec4(col, 1.0);
}

