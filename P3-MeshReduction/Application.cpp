#define GLM_EXT_INCLUDED
#include "Application.h"
#include "glm/gtc/matrix_transform.hpp"


Application::Application(VulkanInstance vkInstance) {
	vk = vkInstance;

//Project Specific Initialization
	std::vector<primative::Vertex> verticies {
		{ { -0.5f, -0.5f } },
		{ {  0.5f,  0.5f } },
		{ { -0.5f,  0.5f } },
		{ { -0.5f, -0.5f } },
		{ {  0.5f, -0.5f } },
		{ {  0.5f,  0.5f } },
	};

	vk.CreateVertexBuffers(verticies);
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
	vkCmdBindPipeline(vk.drawCmd, VK_PIPELINE_BIND_POINT_GRAPHICS, vk.graphicsPipeline);

	glm::vec3 cameraPos = { 0.0f, 0.0f, -1.0f };
	glm::vec3 cameraForward = { 0.0f, 0.0f, 1.0f };
	glm::vec3 cameraUp = { 0.0f, 1.0f, 0.0f };

	glm::mat4 viewProj =
		glm::perspective(45.0f, 4.0f / 3.0f, .01f, 100.0f)
		* glm::lookAt(cameraPos, cameraForward + cameraPos, cameraUp);

	/*
	primative::uniform uni;
	glm::vec3 cameraPos = { 0.0f, 0.0f, -15.0f };
	glm::vec3 cameraForward = { 0.0f, 0.0f, 1.0f };
	glm::vec3 cameraUp = { 0.0f, 1.0f, 0.0f };

	glm::mat4 model = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	model = glm::translate(model, glm::vec3(1.0, 0, 0));

	uni.modelViewProj = 
		glm::perspective(50.0f, 4.0f / 3.0f, .01f, 100.0f)
		* glm::lookAt(cameraPos, cameraForward + cameraPos, cameraUp)
		* model
	;

	vkCmdPushConstants(vk.drawCmd, vk.pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(primative::uniform), &uni);

	vkCmdDraw(vk.drawCmd, 6, 1, 0, 0);*/

	vk.endSetCmdBuffer(vk.drawCmd);

	vk.drawFrame();
}
