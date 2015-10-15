uniform vec2 offset;
uniform sampler2D fonttex;

attribute vec2 Vertex;
attribute vec2 Texcoord;
attribute vec4 Color;

varying vec2 tc;
varying vec4 col;

void main()
{
	tc = Texcoord;
	col = Color;
	
	gl_Position = vec4(Vertex.xy + offset.xy, 0.0, 1.0);
}


