#include "Normal.h"

using namespace Service::Model;

Normal::Normal(Vertex v[])
{
	this->Nx = (v[1].y - v[0].y) * (v[2].z - v[0].z) - (v[1].z - v[0].z) * (v[2].y - v[0].y);
	this->Ny = (v[1].z - v[0].z) * (v[2].x - v[0].x) - (v[1].x - v[0].x) * (v[2].z - v[0].z);
	this->Nz = (v[1].x - v[0].x) * (v[2].y - v[0].y) - (v[1].y - v[0].y) * (v[2].x - v[0].x);
}

Normal::Normal() { }

Normal Normal::Normalize()
{
	Normal normal;

	float length = Length();

	normal.Nx = this->Nx / length;
	normal.Ny = this->Ny / length;
	normal.Nz = this->Nz / length;

	return normal;
}

float Normal::Length()
{
	return sqrt((Nx * Nx) + (Ny * Ny) + (Nz * Nz));
}
