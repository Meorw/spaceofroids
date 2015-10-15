#version 150

in vec2 Vertex;

void main()
{
	gl_Position = vec4(Vertex.xy, 0.0, 1.0);
}

