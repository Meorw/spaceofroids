#version 150

/* Per vertex Lambert shader + texturing */

/* variables per vertex
*/
in vec3 Vertex;
in vec3 Normal;
in vec3 Texcoord;
in vec3 Tangent;
in vec3 Binormal;

/* variables per primitive
*/
uniform mat4 World;					/* transform to world space */
uniform mat4 WorldViewProjection;	/* transform to screen space */
uniform mat4 WorldInverseTranspose;	/* transform to world space (normals) */

uniform vec3 ViewPosition;
uniform vec3 LightPosition;
uniform vec3 LightColor;

/* 'out' variables are interpolated over the triangle
	and available per pixel in the fragment shader
 */
out vec2 texcoord;
out vec3 tangent;
out vec3 binormal;
out vec3 normal;
out vec3 lightVector;


void main() {

	vec3 worldPosition = (World * vec4(Vertex.xyz, 1.0)).xyz;
	vec3 worldNormal = (WorldInverseTranspose * vec4(Normal.xyz, 1.0)).xyz;
	vec3 viewVector = normalize(ViewPosition - worldPosition);

	/* diffuse shading
	*/
	lightVector = normalize(LightPosition - worldPosition);

	texcoord = vec2(Texcoord.x, Texcoord.y) * 2f;

	tangent = (WorldInverseTranspose * vec4(Tangent.xyz, 1.0)).xyz;
	binormal = (WorldInverseTranspose * vec4(Binormal.xyz, 1.0)).xyz;
	normal = (WorldInverseTranspose * vec4(Normal.xyz, 1.0)).xyz;

	/* set the final position for this vertex
	*/
	gl_Position = WorldViewProjection * vec4(Vertex.xyz, 1.0);

}



