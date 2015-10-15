#version 150

uniform vec2 offset;
uniform sampler2D fonttex;

in vec2 tc;
in vec4 col;

out vec4 fragColor;

void main()
{
	vec4 tex = texture2D(fonttex, tc);
	fragColor = vec4(col.xyz, tex.r * col.w);
}


