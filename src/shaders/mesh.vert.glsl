#version 450

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inUV;

layout(binding = 0) uniform MVP {
    mat4 mvp;
} ubo;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragUV;

void main() {
    gl_Position = ubo.mvp * vec4(inPos, 1.0);
    fragColor = inColor;
    fragUV = inUV;
}