#version 150

/* diffuse texture sampler
*/
uniform sampler2D DiffuseTex;

uniform vec3 Ambient;

in vec2 texcoord;
in vec3 diffuse_color;

out vec4 fragColor;

void main() {

	vec4 color = vec4(Ambient.xyz, 1.0) + vec4(diffuse_color, 1.0);

	/* texture lookup
    */
	color *= texture2D(DiffuseTex, texcoord);

	/* write final color to buffer
    */
	fragColor = color;

}
