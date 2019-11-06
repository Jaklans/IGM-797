#include "Mesh.h"
#include <fstream>
#include <sstream>
#include <iostream>


void Mesh::GenerateFromFile(const char* filename){
	//verticies = std::vector<primative::Vertex>();
	//indices = std::vector<unsigned short>();
	std::vector<Vertex> tempVerts = std::vector<Vertex>();
	std::vector<Triangle> triangles = std::vector<Triangle>();

	std::ifstream inputStream(filename);

	char lineType;
	std::string line;

	Vertex currentVertex;
	currentVertex.neighbors = std::vector<unsigned short>();
	currentVertex.triangles = std::vector<Triangle*>();

	Triangle currentTri;
	unsigned short currentIndicies[3];

	//TEMP - using normal to send color data
	//currentVertex.normal = { 0.0f, 0.0f, 0.0f };

	size_t i;
	while (std::getline(inputStream, line)) {
		std::istringstream lineStream(line);

		lineStream >> lineType;

		switch (lineType) {
		case 'v':
		{
			// read a vertex
			lineStream >> currentVertex.position.x;
			lineStream >> currentVertex.position.y;
			lineStream >> currentVertex.position.z;
			currentVertex.index++;

			//if (currentVertex.normal.r < 1.0f) {
			//	currentVertex.normal.r += .0015f;
			//}
			//else if (currentVertex.normal.g < 1.0f) {
			//	currentVertex.normal.g += .0015f;
			//}
			//else if (currentVertex.normal.b < 1.0f) {
			//	currentVertex.normal.b += .0015f;
			//}

			tempVerts.push_back(currentVertex);
			break;
		}
		case 'f':
		{
			currentTri = Triangle();

			// read a triangle's vertex indices
			lineStream >> currentIndicies[0];
			lineStream >> currentIndicies[1];
			lineStream >> currentIndicies[2];
			currentIndicies[0]--;
			currentIndicies[1]--;
			currentIndicies[2]--;
			currentTri.verticies[0] = currentIndicies[0];
			currentTri.verticies[1] = currentIndicies[1];
			currentTri.verticies[2] = currentIndicies[2];
			currentTri.calculateNormal(&tempVerts);

			triangles.push_back(currentTri);
			Triangle* thisTri = &triangles[triangles.size() - 1];
			
			bool found;
			//For each vertex
			for (i = 0; i < 3; i++) {
				tempVerts[currentTri.verticies[i]].triangles.push_back(thisTri);

				//Try to add the other two indicies
				for (size_t j = 0; j < 3; j++) {
					if (i == j) continue;

					found = false;
					//Look through each of its neighbors  
					for (size_t k = 0; k < tempVerts[currentTri.verticies[i]].neighbors.size(); k++) {
						if (tempVerts[currentTri.verticies[i]].neighbors[k] == currentIndicies[j]) {
							found = true;
							break;
						}
					}

					//And add this index if it is not there already
					if (!found) tempVerts[currentTri.verticies[i]].neighbors.push_back(currentIndicies[j]);
				}
			}
			//indices.push_back(currentIndicies[0] - 1);
			//indices.push_back(currentIndicies[1] - 1);
			//indices.push_back(currentIndicies[2] - 1);
			break;
		}
		default:
			// skip the line
			break;
		}
	}
	inputStream.close();

	for (i = 0; i < tempVerts.size(); i++) {
		tempVerts[i].GenerateAvgNormal();
		tempVerts[i].index = i;
		for (size_t j = 0; j < tempVerts.size(); j++) {
			if (i == j) continue;
			ComputeEdgeCost(&tempVerts[i], &tempVerts[j]);
		}
	}

	verticies = std::vector<primative::Vertex>(tempVerts.size());
	size_t vItt = tempVerts.size() - 1;
	verticies[0].pos.x = 0.0f;
	indices = std::vector<unsigned short>(triangles.size() * 3);
	size_t iItt = triangles.size() * 3 - 1;
	indices[0] = 0;

	unsigned short v = 0;
	float cost;
	primative::Vertex currentVert;
	

	unsigned short* indexMapping = new unsigned short[tempVerts.size()];

	while (!indices[0] && verticies[0].pos.x == 0.0f) {
		cost = 100000000;
		for (i = 0; i < tempVerts.size(); i++) {
			if (tempVerts[i].collapse && cost > tempVerts[i].cost) {
				v = i;
			}
		}

		//Collapse the vertex
		currentVert.pos = tempVerts[v].position;
		currentVert.normal = tempVerts[v].normal;

		if (!tempVerts[v].collapse) {
			//Send V to the list
			indexMapping[tempVerts[v].index] = vItt;
			verticies[vItt--] = currentVert;

			continue;
		}

		for (i = 0; i < tempVerts[v].triangles.size(); i++) {
			if (tempVerts[v].triangles[i]->containVertex(tempVerts[v].collapse)) {
				//Tri contains edge, remove it
				indices[iItt--] = tempVerts[tempVerts[v].triangles[i]->verticies[2]].index;
				indices[iItt--] = tempVerts[tempVerts[v].triangles[i]->verticies[1]].index;
				indices[iItt--] = tempVerts[tempVerts[v].triangles[i]->verticies[0]].index;
			}
			else {
				//Replace V with the vert we collapse to
				if (tempVerts[v].triangles[i]->verticies[0] == v) tempVerts[v].triangles[i]->verticies[0] = tempVerts[v].collapse;
				if (tempVerts[v].triangles[i]->verticies[1] == v) tempVerts[v].triangles[i]->verticies[0] = tempVerts[v].collapse;
				if (tempVerts[v].triangles[i]->verticies[2] == v) tempVerts[v].triangles[i]->verticies[0] = tempVerts[v].collapse;
			}
		}

		tempVerts[v].collapse = -1;

		for (i = 0; i < tempVerts[v].neighbors.size(); i++) {
			ComputeVertexCollapseCost(&tempVerts[tempVerts[v].neighbors[i]], tempVerts);
		}
	}

	for (i = 0; i < indices.size(); i++) {
		indices[i] = indexMapping[indices[i]];
	}

	delete[] indexMapping;
}


void Mesh::Render(VkCommandBuffer drawCmd, unsigned short triCount){
	if (triCount > indices.size() / 3) {
		triCount = indices.size() / 3;
	}

	vkCmdDrawIndexed(drawCmd, triCount * 3, 1, 0, 0, 0);
}

float ComputeEdgeCost(Vertex* u, Vertex* v) {
	std::vector<Triangle*> sides;
	for (int i = 0; i < u->triangles.size(); i++) {
		if (u->triangles[i]->containVertex(v->index)) {
			sides.push_back(u->triangles[i]);
		}
	}

	if (sides.size() < 2) {
		//std::cout << "not 2 edges";
		return 0.0f;
	}

	return glm::dot(sides[0]->normal, sides[1]->normal) * glm::length(u->position - v->position);
}

void ComputeVertexCollapseCost(Vertex* v, std::vector<Vertex>& verts) {
	v->collapse = 0;

	if (v->neighbors.size() <= 0) {
		v->cost = -.01f;
		return;
	}

	v->cost = 10000000.0f;
	for (int i = 0; i < v->neighbors.size(); i++) {
		if (verts[v->neighbors[i]].collapse == -1) continue;
		float cost = ComputeEdgeCost(v, &verts[v->neighbors[i]]);
		if (cost < v->cost) {
			v->collapse = verts[v->neighbors[i]].index;
			v->cost = cost;
		}
	}
}

void Vertex::GenerateAvgNormal(){
	normal = { 0.0f, 0.0f, 0.0f };
	for(size_t i = 0; i < triangles.size(); i++){
		normal += triangles[i]->normal;
	}
	normal = glm::normalize(normal);
}
