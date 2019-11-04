#pragma once
#define GLM_EXT_INCLUDED
#include "Application.h"
#include "glm/gtc/matrix_transform.hpp"


Application::Application(VulkanInstance vkInstance) {
	vk = vkInstance;

//Project Specific Initialization
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

	vk.endSetCmdBuffer(vk.drawCmd);

	vk.drawFrame();
}
