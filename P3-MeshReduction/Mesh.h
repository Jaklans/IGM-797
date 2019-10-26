#pragma once
#include "Primative.h"
#include <vector>
#include <iostream>


struct Mesh {
	std::vector<primative::Vertex> verticies;
	std::vector<unsigned short> indices;

	void GenerateFromFile(const char* filename);
	void InitialMeshReductionSort();
	void Render(VkCommandBuffer drawCmd, unsigned short triCount = 0xffff);
};

struct Triangle;

struct Vertex {
	glm::vec3 position;
	unsigned short index;
	std::vector<Vertex*> neighbors;
	std::vector<Triangle*> adjacentTris;
	float cost;
	Vertex* collapse;
};

struct Triangle {
	Vertex* verticies[3];
	glm::vec3 normal;

	bool containVertex(Vertex* vert) {
		return verticies[0] == vert || verticies[1] == vert || verticies[2] == vert;
	}
};

float ComputeEdgeCost(Vertex* u, Vertex* v);

void ComputeVertexEdgeCost(Vertex* v);

void Collapse(Vertex* u, Vertex* v) {
	if (!v) {
		//delete u
		return;
	}
	int i;
	std::vector<Vertex*> temp;
	for (i = 0; i < u->neighbors.size(); i++) {
		temp.push_back(u->neighbors[i]);
	}

	for (i = u->adjacentTris.size(); i >= 0; i--) {
		if (u->adjacentTris[i]->containVertex(v)) {
			//delete 
		}
	}
}