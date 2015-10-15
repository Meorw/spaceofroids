#version 150

in vec2 Vertex;
in vec2 Texcoord;
in vec4 Color;

uniform vec2 offset;
uniform sampler2D fonttex;

out vec2 tc;
out vec4 col;

void main()
{
	tc = Texcoord;
	col = Color;
	
	gl_Position = vec4(Vertex.xy + offset.xy, 0.0, 1.0);
}


