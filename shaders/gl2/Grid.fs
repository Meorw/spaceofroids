uniform vec3 Color;

void main()
{
	gl_FragColor = vec4(Color.xyz, 1.0);
}

