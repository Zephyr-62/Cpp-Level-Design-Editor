#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;

uniform mat4 engine_mvp_mat;
uniform mat3 engine_normal_mat;

out vec3 vNormal;   // world space
out vec2 vUV;

void main() {
    vNormal = engine_normal_mat * aNormal;
    vUV = aUV;
    gl_Position = engine_mvp_mat * vec4(aPos, 1.0);
}