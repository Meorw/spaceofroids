#version 150

/* diffuse texture sampler
*/
uniform sampler2D DiffuseTex;
uniform sampler2D BumpMap;

uniform vec3 Ambient;
uniform vec3 LightColor;

uniform mat4 WorldInverseTranspose;	/* transform to world space (normals) */

in vec2 texcoord;
in vec3 tangent;
in vec3 binormal;
in vec3 normal;
in vec3 lightVector;

out vec4 fragColor;

void main() {
    vec3 bump = 2 * texture2D(BumpMap, texcoord).rgb - 1;
    float alpha = bump.x;
    float beta = bump.y;
    float gamma = bump.z;

    vec3 n = alpha * normalize(tangent) 
            + beta * normalize(binormal) 
            + gamma * normalize(normal);
    //n = normal;

    float lightNormalCos = max(0.0, dot(normalize(n), normalize(lightVector)));
	vec3 diffuse_color = LightColor * lightNormalCos;

	vec4 color = vec4(Ambient.xyz, 1.0) + vec4(diffuse_color, 1.0);

	/* texture lookup
    */
	color *= texture2D(DiffuseTex, texcoord);

    /* specular shine */

	/* write final color to buffer
    */
	fragColor = color;

}
