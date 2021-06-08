#include "Mesh.h"
#include <fstream>
#include <sstream>
#include <iostream>


void Mesh::GenerateFromFile(const char* filename){

	std::ifstream inputStream(filename);

	char lineType;
	std::string line;

	int vertexCount = 0;
	int faceCount = 0;

	//Count the file first
	while (std::getline(inputStream, line)) {
		std::istringstream lineStream(line);

		lineStream >> lineType;

		switch (lineType) {
		case 'v':
		{	vertexCount++; break; }
		case 'f':
		{ faceCount++; break; }
		}
	}

	Vertex* verts = new Vertex[vertexCount]; 
	Triangle* tris = new Triangle[faceCount];

	int vi = 0;
	int ti = 0;

	inputStream.clear();
	inputStream.seekg(0);

	size_t totalLinecount = vertexCount + faceCount;
	//This only works if all verticies are defined before faces, and faces are only triangles
	for (size_t l = 0; l < totalLinecount; l++) {
		std::getline(inputStream, line);
		std::istringstream lineStream(line);
		
		lineStream >> lineType;

		switch (lineType) {
		case 'v':
		{
			// read a vertex
			lineStream >> verts[vi].position.x;
			lineStream >> verts[vi].position.y;
			lineStream >> verts[vi].position.z;
			verts[vi].index = vi;
			vi++;

			break;
		}
		case 'f':
		{
			Triangle* currentTri = tris + ti;

			unsigned short currentIndicies[3];

			// read a triangle's vertex indices
			unsigned short temp;
			lineStream >> currentIndicies[0];
			lineStream >> currentIndicies[1];
			lineStream >> currentIndicies[2];

			//Indicies are not zero indexed
			currentTri->verticies[0] = --currentIndicies[0];
			currentTri->verticies[1] = --currentIndicies[1];
			currentTri->verticies[2] = --currentIndicies[2];
			currentTri->calculateNormal(verts);

			
			//For each vertex
			
			for (size_t i = 0; i < 3; i++) {
				Triangle* ref = currentTri;
				verts[currentTri->verticies[i]].triangles.push_back(ref);
			}
/*
				//Try to add the other two indicies
				for (size_t j = 0; j < 3; j++) {
					if (i == j) continue;


					bool found = false;
					//Loop through existing indicies to make sure the new one is not already present
					for (size_t k = 0; k < verts[currentTri->verticies[i]].neighbors.size(); k++) {
						if (verts[currentTri->verticies[i]].neighbors[k] == currentIndicies[j]) {
							found = true;
							break;
						}
					}

					//And add this index if it is not there already
					if (!found) verts[currentTri->verticies[i]].neighbors.push_back(currentIndicies[j]);
				}
			}*/

			ti++;

			break;
		}
		default:
			// skip the line
			break;
		}
	}
	inputStream.close();

	for (size_t i = 0; i < vertexCount; i++) {
		verts[i].GenerateAvgNormal();
		for (size_t j = 0; j < vertexCount; j++) {
			if (i == j) continue;
			//ComputeEdgeCost(&verts[i], &verts[j]);
		}
	}

	//Render the bunny TEMP

	verticies = std::vector<primative::Vertex>(vertexCount);
	indices = std::vector<unsigned short>(faceCount * 3);
	 
	for (size_t i = 0; i < vertexCount; i++) {
		verticies[i].pos = verts[i].position;
		verticies[i].normal = verts[i].normal;
	}
	for (size_t i = 0; i < faceCount; i++) {
		indices[i * 3 + 0] = tris[i].verticies[0];
		indices[i * 3 + 1] = tris[i].verticies[1];
		indices[i * 3 + 2] = tris[i].verticies[2];
	}
	
	delete[] tris;
	delete[] verts;
	

	return;

	//Render the bunny TEMP

	/*
	verticies = std::vector<primative::Vertex>(verts.size());
	size_t vItt = verts.size() - 1;
	verticies[0].pos.x = 0.0f;
	indices = std::vector<unsigned short>(tris.size() * 3);
	size_t iItt = tris.size() * 3 - 1;
	indices[0] = 0;

	unsigned short v = 0;
	float cost;
	primative::Vertex currentVert;
	

	unsigned short* indexMapping = new unsigned short[verts.size()];

	while (!indices[0] && verticies[0].pos.x == 0.0f) {
		cost = 100000000;
		for (i = 0; i < verts.size(); i++) {
			if (verts[i].collapse && cost > verts[i].cost) {
				v = i;
			}
		}

		//Collapse the vertex
		currentVert.pos = verts[v].position;
		currentVert.normal = verts[v].normal;

		if (!verts[v].collapse) {
			//Send V to the list
			indexMapping[verts[v].index] = vItt;
			verticies[vItt--] = currentVert;

			continue;
		}

		for (i = 0; i < verts[v].triangles.size(); i++) {
			if (verts[v].triangles[i]->containVertex(verts[v].collapse)) {
				//Tri contains edge, remove it
				indices[iItt--] = verts[verts[v].triangles[i]->verticies[2]].index;
				indices[iItt--] = verts[verts[v].triangles[i]->verticies[1]].index;
				indices[iItt--] = verts[verts[v].triangles[i]->verticies[0]].index;
			}
			else {
				//Replace V with the vert we collapse to
				if (verts[v].triangles[i]->verticies[0] == v) verts[v].triangles[i]->verticies[0] = verts[v].collapse;
				if (verts[v].triangles[i]->verticies[1] == v) verts[v].triangles[i]->verticies[0] = verts[v].collapse;
				if (verts[v].triangles[i]->verticies[2] == v) verts[v].triangles[i]->verticies[0] = verts[v].collapse;
			}
		}

		verts[v].collapse = -1;

		for (i = 0; i < verts[v].neighbors.size(); i++) {
			ComputeVertexCollapseCost(&verts[verts[v].neighbors[i]], verts);
		}
	}

	for (i = 0; i < indices.size(); i++) {
		indices[i] = indexMapping[indices[i]];
	}

	delete[] indexMapping;
	*/
}


void Mesh::Render(VkCommandBuffer drawCmd, unsigned short triCount){
	if (triCount > indices.size() / 3) {
		triCount = indices.size() / 3;
	}

	vkCmdDrawIndexed(drawCmd, triCount * 3, 1, 0, 0, 0);
}

/*
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
}*/

void Vertex::GenerateAvgNormal(){
	normal = { 0.0f, 0.0f, 0.0f };
	for(size_t i = 0; i < triangles.size(); i++){
		normal += triangles[i]->normal;
	}
	normal = glm::normalize(normal);
}
