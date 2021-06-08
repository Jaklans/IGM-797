#pragma once
#include "Primative.h"
#include <vector>
#include <iostream>


struct Mesh {
	std::vector<primative::Vertex> verticies;
	std::vector<unsigned short> indices;

	void GenerateFromFile(const char* filename);
	void Render(VkCommandBuffer drawCmd, unsigned short triCount = 0xffff);
};

struct Triangle;

class Vertex {
public:
	glm::vec3 position;
	glm::vec3 normal;
	unsigned short index;
	//std::vector<unsigned short> neighbors;
	std::vector<Triangle*> triangles;
	float cost;
	//If MAX USHORT, uninitialized or inactive
	unsigned short collapse;

	Vertex() {
		position = { 0.0f, 0.0f, 0.0f };
		normal = { 0.0f, 0.0f, 0.0f };
		index = 0;
		//neighbors = std::vector<unsigned short>();
		triangles = std::vector<Triangle*>();
		cost = -1.0f;
		collapse = -1;
	}


	void GenerateAvgNormal();
};

struct Triangle {
	unsigned short verticies[3];
	glm::vec3 normal;

	Triangle() {
		verticies[0] = 0;
		verticies[1] = 0;
		verticies[2] = 0;

		normal = { 0.0f, 0.0f, 0.0f };
	}

	bool containVertex(unsigned short vert) {
		return verticies[0] == vert || verticies[1] == vert || verticies[2] == vert;
	}

	void calculateNormal(Vertex* verts) {
		normal = -glm::normalize(glm::cross(
			verts[verticies[1]].position - verts[verticies[0]].position,
			verts[verticies[1]].position - verts[verticies[2]].position));
	}
};

float ComputeEdgeCost(Vertex* u, Vertex* v);

void ComputeVertexCollapseCost(Vertex* v, std::vector<Vertex>& verts);