attribute vec3 Vertex;
attribute vec2 Texcoord;

uniform mat4 WorldViewProjection;

varying vec2 texcoord;

void main() {
	texcoord = Texcoord;
	gl_Position = WorldViewProjection * vec4(Vertex.xyz, 1.0);
}



