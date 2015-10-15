attribute vec3 Vertex;
attribute vec3 Normal;
attribute vec3 Texcoord;

uniform mat4 ViewProjection;
uniform vec3 ViewPosition;

uniform vec3 worldpos;
uniform mat3 rotation;

varying vec2 texc;
varying vec3 nrm;

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

