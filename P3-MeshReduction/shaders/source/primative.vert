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

	vec3 lightColor = vec3(1.0f, 0.0f, 1.0f);
	vec3 lightSource = vec3(3.0f, 5.0f, -2.0f);


	vec3 lightDirection = normalize(inPosition - lightSource);
	vec3 diffuse = lightColor * 0.7f * (0.2f + dot(lightDirection, inNormal));

	fragColor = vec4(diffuse, 1.0f);
	fragColor = vec4((inNormal + 1.0f) / 2.0f + vec3(0.2f, 0.2f, 0.2f), 1.0f);
}