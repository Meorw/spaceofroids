#version 150

in vec3 Vertex;
in vec3 Normal;
in vec3 Texcoord;

uniform mat4 ViewProjection;

uniform vec3 lcolor;
uniform vec3 worldpos;

out vec2 texc;
out vec3 nrm;

void main()
{
	vec4 sp = ViewProjection * vec4(worldpos, 1.0);
	sp.xyz /= sp.w;
	vec2 pos = sp.xy + Vertex.xy * 0.002;
	gl_Position = vec4(pos, sp.z, 1.0);
	texc = Texcoord.xy;
	nrm = Normal;
}

