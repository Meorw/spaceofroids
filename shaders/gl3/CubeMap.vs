#version 150

/* Per vertex Lambert shader */

/* variables per vertex
*/
in vec3 Vertex;
in vec3 Normal;

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
out vec3 viewVector;
out vec3 worldNormal;

void main() {

	vec3 worldPosition = (World * vec4(Vertex.xyz, 1.0)).xyz;
	worldNormal = (WorldInverseTranspose * vec4(Normal.xyz, 1.0)).xyz;
	viewVector = normalize(ViewPosition - worldPosition);

	/* set the final position for this vertex
	*/
	gl_Position = WorldViewProjection * vec4(Vertex.xyz, 1.0);
}

