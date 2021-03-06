attribute vec3 Vertex;
attribute vec3 Normal;
attribute vec3 Texcoord;

uniform mat4 WorldViewProjection;
uniform vec3 col;

void main()
{
	gl_Position = WorldViewProjection * vec4(Vertex.xyz, 1.0);
}

