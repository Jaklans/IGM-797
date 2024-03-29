//Testing and development based on
//https://vulkan-tutorial.com/
//Drawing/Framebuffers

/*TODO:
	Automate Shader Generation
*/

#pragma once
#include <iostream>
#include <stdexcept>
#include "VulkanInstance.h"
#include "imgui/imgui.h"

int main() {
	try {
		VulkanInstance vk;
		vk.Init();
		//ImGui::NewFrame();
		//ImGui::ShowDemoWindow();
		vk.MainLoop();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		system("PAUSE");
		return EXIT_FAILURE;
	}
	system("PAUSE");
	return EXIT_SUCCESS;
}