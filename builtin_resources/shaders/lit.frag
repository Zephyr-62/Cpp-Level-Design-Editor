#version 330 core
in vec3 vNormal;
in vec2 vUV;

uniform vec3 lightDirection;   // direction the light TRAVELS (from light toward scene)
uniform float ambient;
uniform vec3 baseColor;

out vec4 FragColor;

void main() {
    vec3 N = normalize(vNormal);          // interpolation shortens normals, so renormalize
    vec3 L = normalize(-lightDirection);  // flip: we need surface -> light
    float diffuse = max(dot(N, L), 0.0);
    vec3 color = baseColor * (ambient + (1.0 - ambient) * diffuse);
    FragColor = vec4(color, 1.0);
}