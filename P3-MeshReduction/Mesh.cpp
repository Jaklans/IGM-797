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
