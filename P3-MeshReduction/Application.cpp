#pragma once
#define GLM_EXT_INCLUDED
#include "Application.h"
#include "glm/gtc/matrix_transform.hpp"


Application::Application(VulkanInstance vkInstance) {
	vk = vkInstance;

//Project Specific Initialization
	bunnyMesh.GenerateFromFile("assets/models/bunny2k.obj");

	vk.CreateVertexBuffers(bunnyMesh.verticies);
	vk.CreateIndexBuffers(bunnyMesh.indices);
}

void Application::MainLoop() {
	while (!glfwWindowShouldClose(vk.window)) {
		glfwPollEvents();
		Update();
		Render();
	}
}

void Application::Update() {
	
	if (glfwGetKey(vk.window, GLFW_KEY_SPACE)) {
		
	}
}

void Application::Render() {
	vk.beginSetCmdBuffer(vk.drawCmd);
	VkBuffer vertexBuffers[] = { vk.vertexBuffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(vk.drawCmd, 0, 1, vertexBuffers, offsets);
	vkCmdBindIndexBuffer(vk.drawCmd, vk.indexBuffer, 0, VkIndexType::VK_INDEX_TYPE_UINT16);
	vkCmdBindPipeline(vk.drawCmd, VK_PIPELINE_BIND_POINT_GRAPHICS, vk.graphicsPipeline);

	glm::vec3 cameraPos = { 0.0f, 1.0f, -3.0f };
	glm::vec3 cameraForward = { 0.0f, 0.0f, 1.0f };
	glm::vec3 cameraUp = { 0.0f, -1.0f, 0.0f };

	primative::uniform uniform;

	uniform.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	uniform.modelViewProj =
		glm::perspective(45.0f, 4.0f / 3.0f, .01f, 100.0f)
		* glm::lookAt(cameraPos, cameraForward + cameraPos, cameraUp);

	vkCmdPushConstants(vk.drawCmd, vk.pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(primative::uniform), &uniform.modelViewProj);

	bunnyMesh.Render(vk.drawCmd);

	vk.endSetCmdBuffer(vk.drawCmd);

	vk.drawFrame();
}
