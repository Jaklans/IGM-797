#pragma once
#include <glm/glm.hpp>
#include "primative.h"
#include <vector>
#include "VulkanInstance.h"

struct subMesh {
	subMesh* Parent;
	glm::vec3 translation;
	glm::vec3 scale;
	float rotation;
	glm::vec4 color;

	glm::mat4 model;
};

class hirarchicalMesh {
public:
	std::vector<subMesh> meshes;
	void draw(VulkanInstance vk, glm::mat4 viewProj);
};

