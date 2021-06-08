#pragma once
#include "VulkanInstance.h"
#include "Mesh.h"
#include <chrono>

class Application {
	VulkanInstance vk;
	Mesh bunnyMesh;
	float time;
	float dTime;
	std::chrono::time_point<std::chrono::system_clock> startTime;

public:
	Application(VulkanInstance vkInstance);
	void MainLoop();
	void Update();
	void Render();
};