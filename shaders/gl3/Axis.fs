#version 150

uniform vec3 col;

out vec4 fragColor;

void main()
{
	fragColor = vec4(col.xyz, 1.0);
}

