#pragma once
#define GLM_EXT_INCLUDED
#include "Application.h"
#include "glm/gtc/matrix_transform.hpp"


Application::Application(VulkanInstance vkInstance) {
	vk = vkInstance;

//Project Specific Initialization
	vk.beginSetCmdBuffer(vk.compCmd);
	vkCmdBindPipeline(vk.compCmd, VK_PIPELINE_BIND_POINT_COMPUTE, vk.compPipeline);

	//vkCmdBindDescriptorSets(
	//	vk.compCmd, VK_PIPELINE_BIND_POINT_COMPUTE, 
	//	vk.compPipelineLayout, 
	//	0, 1, 
	//	&vk.compDescriptorSetLayout, 
	//	0, 0);
	
	vk.endSetCmdBuffer(vk.compCmd);

	vk.beginSetCmdBuffer(vk.drawCmd);
	vkCmdBindPipeline(vk.drawCmd, VK_PIPELINE_BIND_POINT_GRAPHICS, vk.graphicsPipeline);
	vk.endSetCmdBuffer(vk.drawCmd);
}

void Application::MainLoop() {
	while (!glfwWindowShouldClose(vk.window)) {
		glfwPollEvents();
		Update();
		Render();
	}
}

void Application::Update() {
}

void Application::Render() {
	//vk.beginSetCmdBuffer(vk.drawCmd);
	//VkBuffer vertexBuffers[] = { vk.vertexBuffer };
	//VkDeviceSize offsets[] = { 0 };
	//vkCmdBindVertexBuffers(vk.drawCmd, 0, 1, vertexBuffers, offsets);
	//vkCmdBindIndexBuffer(vk.drawCmd, vk.indexBuffer, 0, VkIndexType::VK_INDEX_TYPE_UINT16);
	//vkCmdBindPipeline(vk.drawCmd, VK_PIPELINE_BIND_POINT_GRAPHICS, vk.graphicsPipeline);
	//
	//vk.endSetCmdBuffer(vk.drawCmd);
	//
	//vk.drawFrame();
}
