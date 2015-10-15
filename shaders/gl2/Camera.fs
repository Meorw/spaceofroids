varying vec2 texc;
varying vec3 nrm;

void main()
{
	vec3 col = nrm * 0.5 + 0.5;
	gl_FragColor = vec4(col, 1.0);
}

