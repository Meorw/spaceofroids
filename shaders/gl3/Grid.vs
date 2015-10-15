#version 150

in vec2 Vertex;

uniform mat4 WorldViewProjection;

void main()
{
	gl_Position = WorldViewProjection * vec4(Vertex.x, 0.0, Vertex.y, 1.0);
}

