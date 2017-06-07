#include "Triangle.h"

using namespace Service::Modeling;

Triangle::Triangle(Vertex v[])
{
	this->v[0] = v[0];
	this->v[1] = v[1];
	this->v[2] = v[2];

	Normal normal(this->v);
	this->normal = normal;
}

Triangle::Triangle() { }