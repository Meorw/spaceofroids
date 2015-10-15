#version 150

uniform vec3 Ambient;
uniform float Shininess;
uniform vec3 SpecularColor;

in vec3 viewVector;
in vec3 lightVector;
in vec3 worldNormal;
in vec3 diffuse_color;

out vec4 fragColor;

void main() {

	/* write final color to buffer
    */
    vec3 v = normalize(viewVector);
    vec3 l = normalize(lightVector);
    vec3 n = normalize(worldNormal);

	vec3 ref = normalize(reflect(-l, n));
    float spec = pow(max(0, dot(ref, v)), Shininess);
    vec3 specular_color = SpecularColor * spec;
	
    fragColor = 0.1f * vec4(Ambient.xyz, 1.0) 
         + 0.8f*vec4(diffuse_color.xyz, 1.0) 
          + 0.5f*vec4(specular_color.xyz, 1.0);


    //fragColor = vec4(l*0.5+0.5,1);

}