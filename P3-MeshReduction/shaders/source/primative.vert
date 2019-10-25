#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(push_constant) uniform modelViewProj {
	mat4 data;
	vec4 color;
} uniformData;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;

layout(location = 0) out vec4 fragColor;

void main() {
    gl_Position = uniformData.data * vec4(inPosition, 1.0);
    fragColor = vec4(inNormal, 1.0f);
}