#pragma once
#define GLFW_INCLUDE_VULKAN
#include <glm/glm.hpp>
#include <glfw/glfw3.h>
#include <array>

namespace primative {
	struct uniform {
		glm::mat4 modelViewProj;
		glm::vec4 color;

		static VkPushConstantRange getPushConstantRange() {
			VkPushConstantRange pushConstantRange = {};
			pushConstantRange.offset = 0;
			pushConstantRange.size = sizeof(uniform);
			pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

			return pushConstantRange;
		}
	};
	
	struct Vertex {
		glm::vec3 pos;

		static VkVertexInputBindingDescription getBindingDescription() {
			VkVertexInputBindingDescription bindingDescription = {};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(Vertex);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return bindingDescription;
		}
		//might be issue
		static std::array<VkVertexInputAttributeDescription, 1> getAttributeDescriptions() {
			std::array<VkVertexInputAttributeDescription, 1> attributeDescriptions = {};
			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[0].offset = offsetof(Vertex, pos);

			return attributeDescriptions;
		}
	};
}