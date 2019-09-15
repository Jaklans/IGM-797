#pragma once
#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>

#include <vector>

class VulkanInstance {
public:
	GLFWwindow* window;
	VkInstance instance;
	VkDebugUtilsMessengerEXT callback;
	VkPhysicalDevice physicalDevice;
	VkDevice device;
	VkQueue graphicsQueue;
	VkSurfaceKHR surface;
	VkQueue presentQueue;
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	std::vector<VkImageView> swapChainImageViews;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	VkPipelineLayout pipelineLayout;
	VkRenderPass renderPass;
	VkPipeline graphicsPipeline;
	std::vector<VkFramebuffer> swapChainFramebuffers;
	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> sentFrameFences;
	size_t currentFrame = 0;
	bool framebufferResized = false;

	void InitVulkan();
	void Run();

private:
	

	void InitWindow();

	void MainLoop();

	void drawFrame();

	void Cleanup();

	void CleanupSwapchain();

	void recreateSwapchain();

	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

	std::vector<const char*> ValidateExtensions();

	void CreateInstance();

	bool CheckValidationLayerSupport();

	std::vector<const char*> GetRequiredExtensions();

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback);

	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator);

	void SetupDebugCallback();

	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

	int RateDeviceSuitability(VkPhysicalDevice device);

	void PickPhysicalDevice();

	struct QueueFamilyIndices;

	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

	void CreateLogicalDevice();

	void CreateSurface();

	struct SwapChainSupportDetails;

	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);

	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availibleFormats);

	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> availiblePresentmodes);

	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capacities);

	void CreateSwapChain();

	void CreateImageViews();

	void CreateRenderPass();

	VkShaderModule CreateShaderModule(const std::vector<char>& shaderCode);

	void CreatePipeline();

	void CreateFramebuffers();

	void CreateCommandPool();

	void CreateCommandBuffers();

	void CreateSynchronizers();
};
