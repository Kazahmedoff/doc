#include "Builder.h"
#include <vector>

using namespace Service::Model;

float Builder::dx;
float Builder::dy;
float Builder::dz;
short Builder::iso_surface;

// Maps the nodeParity -> vertex indices of the triangle
// -1 means that not a valid Vertex
// Inspired by http://paulbourke.net/geometry/polygonise/ as the mapping is constant

short Builder::getImageValue(short*** voxels, Vertex vertex) {
	short index_x = vertex.x / dx;
	short index_y = vertex.y / dy;
	short index_z = vertex.z / dz;

	return voxels[index_z][index_y][index_x];
}

Builder::Builder() { }

Builder::Builder(float dx, float dy, float dz, short iso_surface)
{
	this->dx = dx;
	this->dy = dy;
	this->dz = dz;
	this->iso_surface = iso_surface;
}

short Builder::getVertexIntensity(short*** voxels, float x, float y, float z, short index) {
	Vertex Vertex = getVertex(x, y, z, index);
	return getImageValue(voxels, Vertex);
}

Vertex Builder::getVertex(float x, float y, float z, short index) {
	switch (index) {
	case 0:
		return Vertex(x, y, z);
	case 1:
		return Vertex(x + dx, y, z);
	case 2:
		return Vertex(x + dx, y, z + dz);
	case 3:
		return Vertex(x, y, z + dz);
	case 4:
		return Vertex(x, y + dy, z);
	case 5:
		return Vertex(x + dx, y + dy, z);
	case 6:
		return Vertex(x + dx, y + dy, z + dz);
	case 7:
		return Vertex(x, y + dy, z + dz);
	default:
		return Vertex(x, y, z);
	}
}

void Builder::getVertices(short edge, short *arr) {
	switch (edge) {
	case 0:
		arr[0] = 0;
		arr[1] = 1;
		break;
	case 1:
		arr[0] = 1;
		arr[1] = 2;
		break;
	case 2:
		arr[0] = 2;
		arr[1] = 3;
		break;
	case 3:
		arr[0] = 3;
		arr[1] = 0;
		break;
	case 4:
		arr[0] = 4;
		arr[1] = 5;
		break;
	case 5:
		arr[0] = 5;
		arr[1] = 6;
		break;
	case 6:
		arr[0] = 6;
		arr[1] = 7;
		break;
	case 7:
		arr[0] = 7;
		arr[1] = 4;
		break;
	case 8:
		arr[0] = 0;
		arr[1] = 4;
		break;
	case 9:
		arr[0] = 1;
		arr[1] = 5;
		break;
	case 10:
		arr[0] = 2;
		arr[1] = 6;
		break;
	case 11:
		arr[0] = 3;
		arr[1] = 7;
		break;
	default:
		arr[0] = 0;
		arr[1] = 1;
	}
}

void Builder::setValues(short*** voxels, float x, float y, float z) {
	for (int i = 0; i < 8; i++)
		nodeParity[i] = getVertexIntensity(voxels, x, y, z, i) < iso_surface;

	for (int i = 0; i < 12; i++) 
	{
		short Vertices[2];
		getVertices(i, Vertices);
		edgeIntersections[i] = nodeParity[Vertices[0]] ^ nodeParity[Vertices[1]];
	}
}

Vertex Builder::getIntersection(short*** voxels, float x, float y, float z, short edge) {
	short Vertices[2];
	getVertices(edge, Vertices);

	Vertex v1 = getVertex(x, y, z, Vertices[0]);
	Vertex v2 = getVertex(x, y, z, Vertices[1]);

	short value1 = getImageValue(voxels, v1);
	short value2 = getImageValue(voxels, v2);

	float scale = (1.0 * (iso_surface - value1)) / (value2 - value1);

	Vertex v;
	v = v1 + (v2 - v1) * scale;

	return v;
}

short Builder::getNodeCaseNumber()
{
	short powerOf2 = 1;
	short caseId = 0;
	for (int i = 0; i < 8; i++) 
	{
		if (nodeParity[i])
			caseId += powerOf2;

		powerOf2 *= 2;
	}

	return caseId;
}

list <Triangle> Builder::getTriangles(short*** voxels, float x, float y, float z) {
	list <Triangle> triangles;
	short index = 0;
	short nodeCase = getNodeCaseNumber();

	while (index < 16 && classicCases[nodeCase][index] != -1) 
	{
		Vertex v[3];

		v[0] = getIntersection(voxels, x, y, z, classicCases[nodeCase][index]);
		v[1] = getIntersection(voxels, x, y, z, classicCases[nodeCase][index + 1]);
		v[2] = getIntersection(voxels, x, y, z, classicCases[nodeCase][index + 2]);

		Triangle triangle(v);
		triangles.push_back(triangle);
		index += 3;
	}
	return triangles;
}