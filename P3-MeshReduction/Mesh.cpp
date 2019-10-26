#include "Mesh.h"
#include <fstream>
#include <sstream>
#include <iostream>


void Mesh::GenerateFromFile(const char* filename){
	verticies = std::vector<primative::Vertex>();
	indices = std::vector<unsigned short>();

	std::ifstream inputStream(filename);

	char lineType;
	std::string line;

	primative::Vertex currentVertex;
	unsigned short currentIndicies[3];

	//TEMP - using normal to send color data
	currentVertex.normal = { 0.0f, 0.0f, 0.0f };

	while (std::getline(inputStream, line)) {
		std::istringstream lineStream(line);

		lineStream >> lineType;

		switch (lineType) {
		case 'v':
		{
			// read a vertex
			lineStream >> currentVertex.pos.x;
			lineStream >> currentVertex.pos.y;
			lineStream >> currentVertex.pos.z;

			if (currentVertex.normal.r < 1.0f) {
				currentVertex.normal.r += .0015f;
			}
			else if (currentVertex.normal.g < 1.0f) {
				currentVertex.normal.g += .0015f;
			}
			else if (currentVertex.normal.b < 1.0f) {
				currentVertex.normal.b += .0015f;
			}

			verticies.push_back(currentVertex);
			break;
		}
		case 'f':
		{
			// read a triangle's vertex indices
			lineStream >> currentIndicies[0];
			lineStream >> currentIndicies[1];
			lineStream >> currentIndicies[2];
			indices.push_back(currentIndicies[0] - 1);
			indices.push_back(currentIndicies[1] - 1);
			indices.push_back(currentIndicies[2] - 1);
			break;
		}
		default:
			// skip the line
			break;
		}
	}
	inputStream.close();
}

void Mesh::InitialMeshReductionSort(){

}

void Mesh::Render(VkCommandBuffer drawCmd, unsigned short triCount){
	if (triCount > indices.size() / 3) {
		triCount = indices.size() / 3;
	}

	vkCmdDrawIndexed(drawCmd, indices.size(), 1, 0, 0, 0);
}

float ComputeEdgeCost(Vertex* u, Vertex* v) {
	std::vector<Triangle*> sides;
	for (int i = 0; i < u->adjacentTris.size(); i++) {
		if (u->adjacentTris[i]->containVertex(v)) {
			sides.push_back(u->adjacentTris[i]);
		}
	}

	if (sides.size() != 2) {
		std::cout << "not 2 edges";
		return 0.0f;
	}

	return glm::dot(sides[0]->normal, sides[1]->normal) * glm::length(u->position - v->position);
}

void ComputeVertexEdgeCost(Vertex* v) {
	v->collapse = 0;

	if (v->neighbors.size() <= 0) {
		v->cost = -.01f;
		return;
	}

	v->cost = 10000000.0f;
	for (int i = 0; i < v->neighbors.size(); i++) {
		float cost = ComputeEdgeCost(v, v->neighbors[i]);
		if (cost < v->cost) {
			v->collapse = v->neighbors[i];
			v->cost = cost;
		}
	}
}
