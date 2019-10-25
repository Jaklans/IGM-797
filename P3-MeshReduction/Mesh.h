#pragma once
#include "Primative.h"
#include <vector>


struct Mesh {
	std::vector<primative::Vertex> verticies;
	std::vector<unsigned short> indices;

	void GenerateFromFile(const char* filename);
	void InitialMeshReductionSort();
	void Render(VkCommandBuffer drawCmd, unsigned short triCount = 0xffff);
};