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

}
