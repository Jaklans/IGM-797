#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(push_constant) uniform modelViewProj {
	mat4 data;
	vec4 color;
} uniformData;

layout(location = 0) in vec2 inPosition;

layout(location = 0) out vec4 fragColor;

void main() {
    gl_Position = uniformData.data * vec4(inPosition, 0.0, 1.0);
    fragColor = uniformData.color;
}