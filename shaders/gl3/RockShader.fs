#version 150

uniform vec3 Ambient;
in vec3 diffuse_color;
in vec3 worldNormal;
in vec3 lightVector;
out vec4 fragColor;

void main() {

	/* write final color to buffer
    */
    const float A = 0.2;
    const float B = 0.6;
    const float C = 0.9;

    float lightNormalCos = max(0.0, dot(normalize(worldNormal), normalize(lightVector)));

	float df = lightNormalCos;
    vec3 color;
    if (df < A)
        color = vec3(1) *0.05;
    else if (df < B) 
        color = vec3(1) * 0.1;
    else if (df < C)
        color = vec3(1) * 0.2;
    else
        color = vec3(1) * 0.4;

	//gl_FragColor = max(vec4(Ambient.xyz, 1.0), vec4(diffuse_color, 1.0));
	fragColor = vec4(vec3(1)*0.05, 1.0) + vec4(color, 1.0);
}
