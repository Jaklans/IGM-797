#include "HirarchicalMesh.h"
#include "glm/gtc/matrix_transform.hpp"

void hirarchicalMesh::draw(VulkanInstance vk, glm::mat4 viewProj)
{
	primative::uniform uniform;

	glm::mat4 translation = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	glm::mat4 rotation = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	glm::mat4 scale = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};


	glm::vec3 zAxis = {
		0.0f, 0.0f, 1.0f
	};

	for(int i = 0; i < meshes.size(); i++) {
		float rotationRad = meshes[i].rotation * glm::pi<float>() / 180;
		
		translation[3] = glm::vec4(meshes[i].translation, 1.0f);

		rotation[0] = glm::vec4(cosf(rotationRad), -sin(rotationRad), 0.0f, 0.0f);
		rotation[1] = glm::vec4(sinf(rotationRad), cos(rotationRad), 0.0f, 0.0f);

		scale[0].x = meshes[i].scale.x;
		scale[1].y = meshes[i].scale.y;

		meshes[i].model = translation * rotation * scale;

		if (meshes[i].Parent) {
			meshes[i].model = meshes[i].Parent->model * meshes[i].model;
		}

		uniform.modelViewProj = meshes[i].model * viewProj;
		uniform.color = meshes[i].color;

		vkCmdPushConstants(vk.drawCmd, vk.pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(primative::uniform), &uniform.modelViewProj);

		vkCmdDraw(vk.drawCmd, 6, 1, 0, 0);
	}
}

