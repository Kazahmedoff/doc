#include <iostream>
#include <fstream>
#include <set>
#include <numeric>
#include <string>

#include "Smoother.h"
#include "IndexedVertex.h"

using namespace std;
using namespace Service::Smoothing;

Smoother::Smoother()
{ }

Smoother::Smoother(Mesh* mesh)
{
	this->mesh= mesh;
}

void Smoother::TaubinSmooth(const float lambda, const float mu, const unsigned short iterations)
{
	cout << "Smoothing Smoother using Taubin lambda|mu algorithm ";
	cout << "(inverse neighbour count weighting)" << "\n";

	for (unsigned short i = 0; i < iterations; ++i)
	{
		cout << "Iteration " << i + 1 << " of " << iterations << "\n";

		laplaceSmooth(lambda);
		laplaceSmooth(mu);
	}
	rebuildMesh();
}

void Smoother::laplaceSmooth(const float scale)
{
	if (mesh != nullptr)
	{
		vector<Vertex>& vertices = mesh->GetUniqueVertices();
		vector<vector<unsigned int>>& vertex_to_vertex_indices = mesh->GetVertexListToVertexIndices();

		Vertex *displacements = new Vertex[vertices.size()];
		for (unsigned int i = 0; i < vertices.size(); ++i)
			displacements[i] = Vertex(0, 0, 0);

		for (unsigned int i = 0; i < vertices.size(); i++)
		{
			if (vertex_to_vertex_indices[i].size() == 0)
				continue;

			float sum = 0.0;
			Vertex *vectors = new Vertex[vertex_to_vertex_indices[i].size()];
			for (unsigned int j = 0; j < vertex_to_vertex_indices[i].size(); ++j)
			{
				unsigned int neighbour_j = vertex_to_vertex_indices[i][j];
				vectors[j] = vertices[neighbour_j] - vertices[i];
				sum += vectors[j].length();
			}

			for (unsigned int j = 0; j < vertex_to_vertex_indices[i].size(); ++j)
			{
				unsigned int neighbour_j = vertex_to_vertex_indices[i][j];
				float length = vectors[j].length();
				displacements[i] += ((vertices[neighbour_j] - vertices[i]) * 1.0f * length) / (1.0f * sum);
			}

			delete[] vectors;
		}

		for (unsigned int i = 0; i < vertices.size(); i++)
			vertices[i] += (displacements[i] * scale);

		delete[] displacements;
	}

	else
	{
		cout << "'MarchingCube' class object is empty!";
	}
}

void Smoother::rebuildMesh()
{
	if (mesh != nullptr)
	{
		vector<Indexed_Triangle>& tr = mesh->GetTrianglesWithIndexedVertices();
		vector<Vertex>& vertices = mesh->GetUniqueVertices();
		list<Triangle>& triangles = mesh->GetTriangleList();

		unsigned int tri_index = 0;
		for (list<Triangle>::iterator triangle = triangles.begin(); triangle != triangles.end(); ++triangle)
		{
			triangle->v[0] = vertices[tr[tri_index].vertex_indices[0]];
			triangle->v[1] = vertices[tr[tri_index].vertex_indices[1]];
			triangle->v[2] = vertices[tr[tri_index].vertex_indices[2]];

			Vertex v1 = triangle->v[1] - triangle->v[0];
			Vertex v2 = triangle->v[2] - triangle->v[0];
			Vertex normal = v1.cross(v2);

			triangle->normal = normal;
			triangle->normal.Normalize();

			tri_index++;
		}
	}

	else
	{
		cout << "'Mesh' class object is empty!";
	}
}

Mesh* Smoother::GetSmoothedMesh()
{
	return this->mesh;
}