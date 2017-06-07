#include "Builder.h"

using namespace Service::Modeling;

// Maps the nodeParity -> vertex indices of the triangle
// -1 means that not a valid Vertex
// Inspired by http://paulbourke.net/geometry/polygonise/ as the mapping is constant

Builder::Builder() { }

Builder::Builder(float dx, float dy, float dz, short iso_surface)
{
	this->dx = dx;
	this->dy = dy;
	this->dz = dz;
	this->iso_surface = iso_surface;
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
	}
}

bool Builder::setValues(short*** voxels, short i, short j, short k) {

	cell.vertex[0].x = i * dx;
	cell.vertex[0].y = j * dy;
	cell.vertex[0].z = k * dz;
	cell.value[0] = voxels[k][j][i];

	cell.vertex[1].x = (i + 1) * dx;
	cell.vertex[1].y = j * dy;
	cell.vertex[1].z = k * dz;
	cell.value[1] = voxels[k][j][i + 1];

	cell.vertex[2].x = (i + 1) * dx;
	cell.vertex[2].y = (j + 1) * dy;
	cell.vertex[2].z = k * dz;
	cell.value[2] = voxels[k][j + 1][i + 1];

	cell.vertex[3].x = i * dx;
	cell.vertex[3].y = (j + 1) * dy;
	cell.vertex[3].z = k * dz;
	cell.value[3] = voxels[k][j + 1][i];

	cell.vertex[4].x = i * dx;
	cell.vertex[4].y = j * dy;
	cell.vertex[4].z = (k + 1) * dz;
	cell.value[4] = voxels[k + 1][j][i];

	cell.vertex[5].x = (i + 1) * dx;
	cell.vertex[5].y = j * dy;
	cell.vertex[5].z = (k + 1) * dz;
	cell.value[5] = voxels[k + 1][j][i + 1];

	cell.vertex[6].x = (i + 1) * dx;
	cell.vertex[6].y = (j + 1) * dy;
	cell.vertex[6].z = (k + 1) * dz;
	cell.value[6] = voxels[k + 1][j + 1][i + 1];

	cell.vertex[7].x = i * dx;
	cell.vertex[7].y = (j + 1) * dy;
	cell.vertex[7].z = (k + 1) * dz;
	cell.value[7] = voxels[k + 1][j + 1][i];

	short count_ = 0;
	for (int i = 0; i < 8; ++i)
	{
		nodeParity[i] = cell.value[i] < iso_surface;

		if (nodeParity[i])
			count_++;
	}

	if (count_ > 0 && count_ < 8)
		return true;

	else
		return false;
}

Vertex Builder::getIntersection(short edge) {
	short vertices[2];
	getVertices(edge, vertices);

	Vertex v1 = cell.vertex[vertices[0]];
	Vertex v2 = cell.vertex[vertices[1]];

	short value1 = cell.value[vertices[0]];
	short value2 = cell.value[vertices[1]];

	if (abs(iso_surface - value1) < 0.1)
		return v1;
	if (abs(iso_surface - value2) < 0.1)
		return v2;
	if (abs(value1 - value2) < 0.1)
		return v1;

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

list <Triangle> Builder::getTriangles() {
	list <Triangle> triangles(0);
	short index = 0;
	short nodeCase = getNodeCaseNumber();

	while (index < 16 && classicCases[nodeCase][index] != -1) 
	{
		Vertex v[3];

		v[0] = getIntersection(classicCases[nodeCase][index]);
		v[1] = getIntersection(classicCases[nodeCase][index + 1]);
		v[2] = getIntersection(classicCases[nodeCase][index + 2]);

		Triangle triangle(v);
		triangle.normal = triangle.normal.Normalize();

		triangles.push_back(triangle);
		index += 3;
	}
	return triangles;
}