#version 150

in vec3 Vertex;
in vec3 Normal;
in vec3 Texcoord;

uniform mat4 WorldViewProjection;
uniform vec3 col;

void main()
{
	gl_Position = WorldViewProjection * vec4(Vertex.xyz, 1.0);
}

