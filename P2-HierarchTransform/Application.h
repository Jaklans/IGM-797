#pragma once
#include "VulkanInstance.h"
#include "HirarchicalMesh.h"

class Application {
	VulkanInstance vk;

	hirarchicalMesh robot;
	size_t meshIndex;
	bool spaceDown;
	glm::vec4 savedColor;

public:
	Application(VulkanInstance vkInstance);
	void MainLoop();
	void Update();
	void Render();
};