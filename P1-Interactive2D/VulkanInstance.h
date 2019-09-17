#pragma once
#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>
#include <glm/glm.hpp>

#include <vector>
#include <array>

class VulkanInstance {
public:
	GLFWwindow* window;
	VkInstance instance;
	VkDebugUtilsMessengerEXT callback;
	VkPhysicalDevice physicalDevice;
	VkDevice device;
	std::vector<uint32_t> queueIndicies;
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
	VkDescriptorPool descriptorPool;
	VkCommandBuffer drawCmd;
	VkCommandBuffer ImGuiCmd;
	std::vector<VkCommandBuffer> commandBuffers;
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> sentFrameFences;
	size_t currentFrame = 0;
	bool framebufferResized = false;

	VkBuffer triVertBuffer;
	VkDeviceMemory triVertBufferMemory;
	VkBuffer lineVertBuffer;
	VkDeviceMemory lineVertBufferMemory;


	void Init();

	void Cleanup();
	
	void MainLoop();

private:
	void InitVulkan();

	void InitWindow();

	void InitImGui();

	void drawFrame();

	//Contains all commands to be run on a standard frame
	void render(VkCommandBuffer cmdBuffer);

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

	void CreateVertexBuffers();

	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	void CreateCommandBuffers();

	void resetCmdBuffer(int index);

	void beginSetCmdBuffer(VkCommandBuffer buffer);

	void endSetCmdBuffer(VkCommandBuffer buffer);

	VkCommandBuffer beginSingleTimeCommands();

	void endSingleTimeCommands(VkCommandBuffer commandBuffer);

	void CreateSynchronizers();

	void CreateDescriptorPool();
};

struct Vertex {
	glm::vec2 pos;
	glm::vec4 color;

	static VkVertexInputBindingDescription getBindingDescription() {
		VkVertexInputBindingDescription bindingDescription = {};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
		std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);
		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		return attributeDescriptions;
	}
};