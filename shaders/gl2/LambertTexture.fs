
/* diffuse texture sampler
*/
uniform sampler2D DiffuseTex;

uniform vec3 Ambient;

varying vec2 texcoord;
varying vec3 diffuse_color;


void main() {

	vec4 color = vec4(Ambient.xyz, 1.0) + vec4(diffuse_color, 1.0);

	/* texture lookup
    */
	color *= texture2D(DiffuseTex, texcoord);

	/* write final color to buffer
    */
	gl_FragColor = color;

}
