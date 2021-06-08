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

	startTime = std::chrono::system_clock::now();
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

	float newTime = (std::chrono::system_clock::now() - startTime).count() / 10000000.0;

	dTime = newTime - time;

	time = newTime;
}


primative::uniform uniform;
bool ff = 1;
void Application::Render() {

	float scaledSpeed = time * 1.0f;
	float distance = 3.0f;

	glm::vec3 cameraPos = {
		0.0f,//cos(scaledSpeed) * distance,
		0.0f,
		-3.0f };//sin(scaledSpeed) * distance };

	glm::vec3 cameraTarget = { 0.0f, 0.0f, 0.0f };
	glm::vec3 cameraForward = glm::normalize(cameraPos - cameraTarget);

	glm::vec3 up = { 0.0f, 1.0f, 0.0f };
	glm::vec3 cameraRight = glm::cross(up, -cameraForward);
	glm::vec3 cameraUp = glm::cross(-cameraForward, cameraRight);

	uniform.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	uniform.modelViewProj =
		glm::perspective(45.0f, 4.0f / 3.0f, .1f, 100.0f)
		* glm::lookAt(cameraPos, cameraForward, -cameraUp);

	VkCommandBuffer* drawBuffer = &vk.commandBuffers[vk.currentFrame];

	vk.beginRenderCmdBuffer(*drawBuffer);

	VkBuffer vertexBuffers[] = { vk.vertexBuffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(*drawBuffer, 0, 1, vertexBuffers, offsets);
	vkCmdBindIndexBuffer(*drawBuffer, vk.indexBuffer, 0, VkIndexType::VK_INDEX_TYPE_UINT16);

	vkCmdPushConstants(*drawBuffer, vk.pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(primative::uniform), &uniform);
	bunnyMesh.Render(*drawBuffer);

	vk.endSetCmdBuffer(*drawBuffer);

	vk.drawFrame();
}
