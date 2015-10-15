#version 150

in vec3 Vertex;
in vec3 Normal;
in vec3 Texcoord;

uniform mat4 ViewProjection;
uniform vec3 ViewPosition;

uniform vec3 worldpos;
uniform mat3 rotation;

out vec2 texc;
out vec3 nrm;

void main()
{
//	vec3 viewvec = worldpos - ViewPosition;
//	float invdist = 1.0 / sqrt(dot(viewvec, viewvec));

//	vec3 pos = Vertex * invdist * 100.0;

//	mat4 rot = World;
//	rot[0] = 

	vec3 pos = rotation * Vertex + worldpos;
	//vec3 pos = Vertex;

	gl_Position = ViewProjection * vec4(pos, 1.0);
	texc = Texcoord.xy;
	nrm = Normal;
}

