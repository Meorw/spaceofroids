#version 150

uniform vec3 Ambient;
in vec3 diffuse_color;
out vec4 fragColor;

void main() {

	/* write final color to buffer
    */
	//gl_FragColor = max(vec4(Ambient.xyz, 1.0), vec4(diffuse_color, 1.0));
	fragColor = vec4(Ambient.xyz, 1.0) + vec4(diffuse_color, 1.0);
}
