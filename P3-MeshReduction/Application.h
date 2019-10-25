#pragma once
#include "VulkanInstance.h"
#include "Mesh.h"

class Application {
	VulkanInstance vk;
	Mesh bunnyMesh;

public:
	Application(VulkanInstance vkInstance);
	void MainLoop();
	void Update();
	void Render();
};