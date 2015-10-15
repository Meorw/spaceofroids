#version 150

uniform vec3 Color;

out vec4 fragColor;

void main()
{
	fragColor = vec4(Color.xyz, 1.0);
}

