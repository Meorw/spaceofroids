
varying vec3 varying_normal;

void main()
{
	vec3 varying_normal_norm = normalize(varying_normal);
	
    /* map the color of this pixel from the normal
    */
	gl_FragColor.a = 1.0;
	gl_FragColor.r = varying_normal_norm.x * 0.5 + 0.5;
	gl_FragColor.g = varying_normal_norm.y * 0.5 + 0.5;
	gl_FragColor.b = varying_normal_norm.z * 0.5 + 0.5;
}
