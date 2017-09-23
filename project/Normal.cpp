#include "Normal.h"

using namespace Service::Modeling;

Normal::Normal(Vertex v[])
{
	this->Nx = (v[1].y - v[0].y) * (v[2].z - v[0].z) - (v[1].z - v[0].z) * (v[2].y - v[0].y);
	this->Ny = (v[1].z - v[0].z) * (v[2].x - v[0].x) - (v[1].x - v[0].x) * (v[2].z - v[0].z);
	this->Nz = (v[1].x - v[0].x) * (v[2].y - v[0].y) - (v[1].y - v[0].y) * (v[2].x - v[0].x);
}

Normal::Normal() { }

void Normal::Normalize()
{
	float length = Length();

	this->Nx = this->Nx / length;
	this->Ny = this->Ny / length;
	this->Nz = this->Nz / length;
}

float Normal::Length()
{
	return sqrt((Nx * Nx) + (Ny * Ny) + (Nz * Nz));
}
