
/* Normals debug shader */

/* variables per vertex
*/
attribute vec3 Vertex;
attribute vec3 Normal;

/* variables per primitive
*/
uniform mat4 WorldViewProjection;	/* transform to screen space */
uniform mat4 World ;				/* tranform to world space */

/* 'varying' variables are interpolated over the triangle
	and available per pixel in the fragment shader
 */
varying vec3 varying_normal;

void main() {

	/* transform the normal of this vertex and set it
		to vary over the triangle
	 */
	vec4 world_normal = World * vec4(Normal, 0.0);
	varying_normal = world_normal.xyz;

	/* set the final position for this vertex
	*/
    gl_Position = WorldViewProjection * vec4(Vertex, 1.0);
}



