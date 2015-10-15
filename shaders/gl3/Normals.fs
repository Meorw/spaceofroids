#version 150

in vec3 varying_normal;

out vec4 fragColor;

void main()
{
	vec3 varying_normal_norm = normalize(varying_normal);
	
    /* map the color of this pixel from the normal
    */
	fragColor.a = 1.0;
	fragColor.r = varying_normal_norm.x * 0.5 + 0.5;
	fragColor.g = varying_normal_norm.y * 0.5 + 0.5;
	fragColor.b = varying_normal_norm.z * 0.5 + 0.5;
}
