#define GLM_EXT_INCLUDED
#include "Application.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_vulkan.h"
#include "imgui/imgui_impl_glfw.h"
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

	robot.meshes = std::vector<subMesh>(11);

	robot.meshes[0] = { //Torso Main
		0,							//Parent
		{ 0.0f, -0.2f, 0.0f },		//Position
		{ 0.2f, 0.2f, 0.0f },		//Scale
		0.0f,						//Rotation
		{ 1.0f, 1.0f, 1.0f, 1.0f }	//Color
	};

	robot.meshes[1] = { //Torso Lower
		&robot.meshes[0],
		{ 0.0f, 1.6f, 0.0f },
		{ 0.8f, 0.8f, 0.0f },
		0.0f,
		{ 0.0f, 1.0f, 0.0f, 1.0f }
	};

	robot.meshes[2] = { //Arm Upper Right
		&robot.meshes[0],
		{ 1.0f, 0.0f, 0.0f },
		{ 0.25f, 0.8f, 0.0f },
		30.0f,
		{ 0.0f, 0.0f, 1.0f, 1.0f }
	};

	robot.meshes[3] = { //Arm Lower Right
		&robot.meshes[2],
		{ 0.0f, 1.75f, 0.0f },
		{ 1.0f, 1.0f, 0.0f },
		0.0f,
		{ 1.0f, 1.0f, 0.0f, 1.0f }
	};
	robot.meshes[4] = { //Arm Upper Left
		&robot.meshes[0],
		{ -1.0f, -0.1f, 0.0f },
		{ 0.25f, 0.8f, 0.0f },
		-30.0f,
		{ 0.0f, 0.0f, 1.0f, 1.0f }
	};

	robot.meshes[5] = { //Arm Lower Left
		&robot.meshes[4],
		{ 0.0f, 1.75f, 0.0f },
		{ 1.0f, 1.0f, 0.0f },
		0.0f,
		{ 1.0f, 1.0f, 0.0f, 1.0f }
	};

	robot.meshes[6] = { //Leg Upper Right
		&robot.meshes[1],
		{ -0.5f, 1.85f, 0.0f },
		{ 0.35f, 1.25f, 0.0f },
		-5.0f,
		{ 0.0f, 0.0f, 1.0f, 1.0f }
	};

	robot.meshes[7] = { //Leg Lower Right
		&robot.meshes[6],
		{ 0.0f, 1.75f, 0.0f },
		{ 1.0f, 1.0f, 0.0f },
		0.0f,
		{ 1.0f, 1.0f, 0.0f, 1.0f }
	};

	robot.meshes[8] = { //Leg Upper Left
		&robot.meshes[1],
		{ 0.5f, 1.85f, 0.0f },
		{ 0.35f, 1.25f, 0.0f },
		5.0f,
		{ 0.0f, 0.0f, 1.0f, 1.0f }
	};

	robot.meshes[9] = { //Leg Lower Left
		&robot.meshes[8],
		{ 0.0f, 1.75f, 0.0f },
		{ 1.0f, 1.0f, 0.0f },
		0.0f,
		{ 1.0f, 1.0f, 0.0f, 1.0f }
	};

	robot.meshes[10] = { //Head
		&robot.meshes[0],
		{ 0.0f, -1.6f, 0.0f },
		{ 0.57f, 0.75f, 0.0f },
		0.0f,
		{ 0.0f, 1.0f, 1.0f, 1.0f }
	};

	meshIndex = 0;
	spaceDown = false;
	savedColor = { 1.0f, 0.0f, 0.0f, 1.0f };
}

void Application::MainLoop() {
	while (!glfwWindowShouldClose(vk.window)) {
		glfwPollEvents();
		Update();
		Render();
	}
}

void Application::Update() {
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	if (glfwGetKey(vk.window, GLFW_KEY_SPACE)) {
		if (!spaceDown) {
			robot.meshes[meshIndex].color = savedColor;
			meshIndex++;
			if (meshIndex >= robot.meshes.size()) meshIndex = 0;
			savedColor = robot.meshes[meshIndex].color;
			robot.meshes[meshIndex].color = { 1.0f, 1.0f, 1.0f, 1.0f };
		}
		spaceDown = true;
	}
	else {
		spaceDown = false;
	}
	if (glfwGetKey(vk.window, GLFW_KEY_A)) {
		robot.meshes[meshIndex].translation.x -= 0.001f;
	}
	if (glfwGetKey(vk.window, GLFW_KEY_D)) {
		robot.meshes[meshIndex].translation.x += 0.001f;
	}
	if (glfwGetKey(vk.window, GLFW_KEY_W)) {
		robot.meshes[meshIndex].translation.y -= 0.001f;
	}
	if (glfwGetKey(vk.window, GLFW_KEY_S)) {
		robot.meshes[meshIndex].translation.y += 0.001f;
	}
	if (glfwGetKey(vk.window, GLFW_KEY_E)) {
		robot.meshes[meshIndex].rotation += 0.01f;
	}
	if (glfwGetKey(vk.window, GLFW_KEY_Q)) {
		robot.meshes[meshIndex].rotation -= 0.01f;
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

	robot.draw(vk, viewProj);
	//robot.meshes[2].rotation+=.00001f;

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
	

	//Set ImGui Command Buffer
	vk.beginSetCmdBuffer(vk.ImGuiCmd);
	ImGui::Render();
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), vk.ImGuiCmd);
	vk.endSetCmdBuffer(vk.ImGuiCmd);

	vk.drawFrame();
}
