
uniform vec3 Ambient;

varying vec3 diffuse_color;


void main() {

	/* write final color to buffer
    */
	gl_FragColor = vec4(Ambient.xyz, 1.0) + vec4(diffuse_color, 1.0);

}
