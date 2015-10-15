uniform vec3 col;

void main()
{
	gl_FragColor = vec4(col.xyz, 1.0);
}

