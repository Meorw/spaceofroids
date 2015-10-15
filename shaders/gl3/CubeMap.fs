#version 150

uniform samplerCube CubeTex;

in vec3 viewVector;
in vec3 worldNormal;

out vec4 fragColor;

void main() {
    vec3 V = normalize(viewVector);
    vec3 N = normalize(worldNormal);
    vec4 texColor = texture(CubeTex, N);
    fragColor = texColor;
}