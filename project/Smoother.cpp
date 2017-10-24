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

Smoother::Smoother(MarchingCube* mc)
{
	marching_cube = mc;
}

//bool Smoother::LoadDataFromBinarySTL(string fileName, const bool generate_triangle_normals, const bool generate_vertex_normals, const unsigned int buffer_width)
//{
//	clear();
//
//	cout << "Reading file: " << fileName << endl;
//
//	ifstream in(fileName.c_str(), ios_base::binary);
//
//	if (in.fail())
//		return false;
//
//	const short header_size = 80;
//	vector<char> buffer(header_size, 0);
//	unsigned int num_triangles = 0; // Must be 4-byte unsigned int.
//
//									// Read header.
//	in.read(reinterpret_cast<char *>(&(buffer[0])), header_size);
//
//	if (header_size != in.gcount())
//		return false;
//
//	if ('s' == tolower(buffer[0]) &&
//		'o' == tolower(buffer[1]) &&
//		'l' == tolower(buffer[2]) &&
//		'i' == tolower(buffer[3]) &&
//		'd' == tolower(buffer[4]))
//	{
//		cout << "Encountered ASCII STL file header -- aborting." << endl;
//		return false;
//	}
//
//	// Read number of triangles.
//	in.read(reinterpret_cast<char *>(&num_triangles), sizeof(unsigned int));
//
//	if (sizeof(unsigned int) != in.gcount())
//		return false;
//
//	tr.resize(num_triangles);
//
//	cout << "Triangles: " << tr.size() << endl;
//
//	// Enough bytes for twelve 4-byte floats plus one 2-byte integer, per triangle.
//	const size_t per_triangle_data_size = (12 * sizeof(float) + sizeof(unsigned short));
//	const size_t buffer_size = per_triangle_data_size * buffer_width;
//	buffer.resize(buffer_size, 0);
//
//	unsigned int num_triangles_remaining = tr.size();
//	unsigned int tri_index = 0;
//	set<Indexed_Vertex> vertex_set;
//
//	while (num_triangles_remaining > 0)
//	{
//		size_t num_triangles_to_read = buffer_width;
//
//		if (num_triangles_remaining < buffer_width)
//			num_triangles_to_read = num_triangles_remaining;
//
//		size_t data_size = per_triangle_data_size*num_triangles_to_read;
//
//		in.read(reinterpret_cast<char *>(&buffer[0]), data_size);
//
//		if (data_size != in.gcount() || in.fail())
//			return false;
//
//		num_triangles_remaining -= num_triangles_to_read;
//
//		// Use a pointer to assist with the copying.
//		// Should probably use std::copy() instead, but memcpy() does the trick, so whatever...
//		char *cp = &buffer[0];
//
//		for (unsigned int i = 0; i < num_triangles_to_read; i++)
//		{
//			// Skip face normal. We will calculate them manually later.
//			cp += 3 * sizeof(float);
//
//			// For each of the three vertices in the triangle.
//			for (short unsigned int j = 0; j < 3; j++)
//			{
//				Indexed_Vertex v;
//
//				// Get vertex components.
//				memcpy(&v.x, cp, sizeof(float)); cp += sizeof(float);
//				memcpy(&v.y, cp, sizeof(float)); cp += sizeof(float);
//				memcpy(&v.z, cp, sizeof(float)); cp += sizeof(float);
//
//				// Look for vertex in set.
//				set<Indexed_Vertex>::const_iterator find_iter = vertex_set.find(v);
//
//				// If vertex not found in set...
//				if (vertex_set.end() == find_iter)
//				{
//					// Assign new vertices index
//					v.index = vertices.size();
//
//					// Add vertex to set
//					vertex_set.insert(v);
//
//					// Add vertex to vector
//					Vertex vertex;
//					vertex.x = v.x;
//					vertex.y = v.y;
//					vertex.z = v.z;
//					vertices.push_back(vertex);
//
//					// Assign vertex index to triangle
//					tr[tri_index].vertex_indices[j] = v.index;
//
//					// Add triangle index to vertex
//					vector<unsigned int> tri_indices;
//					tri_indices.push_back(tri_index);
//					vertex_to_triangle_indices.push_back(tri_indices);
//				}
//				else
//				{
//					// Assign existing vertex index to triangle
//					tr[tri_index].vertex_indices[j] = find_iter->index;
//
//					// Add triangle index to vertex
//					vertex_to_triangle_indices[find_iter->index].push_back(tri_index);
//				}
//			}
//
//			// Skip attribute.
//			cp += sizeof(unsigned short);
//
//			tri_index++;
//		}
//	}
//
//	vertex_to_vertex_indices.resize(vertices.size());
//
//	for (unsigned int i = 0; i < vertex_to_triangle_indices.size(); i++)
//	{
//		// Use a temporary set to avoid duplicates.
//		set<unsigned int> vertex_to_vertex_indices_set;
//
//		for (unsigned int j = 0; j < vertex_to_triangle_indices[i].size(); j++)
//		{
//			unsigned int tri_index = vertex_to_triangle_indices[i][j];
//
//			for (short k = 0; k < 3; k++)
//				if (i != tr[tri_index].vertex_indices[k]) // Don't add current vertex index to its own adjacency list.
//					vertex_to_vertex_indices_set.insert(tr[tri_index].vertex_indices[k]);
//		}
//
//		// Copy to final vector.
//		for (set<unsigned int>::const_iterator ci = vertex_to_vertex_indices_set.begin(); ci != vertex_to_vertex_indices_set.end(); ci++)
//			vertex_to_vertex_indices[i].push_back(*ci);
//	}
//
//	cout << "Vertices:     " << tr.size() * 3 << " (of which " << vertices.size() << " are unique)" << endl;
//
//	in.close();
//
//	if (generate_triangle_normals == true)
//	{
//		generateTriangleNormals();
//	}
//
//	if (generate_vertex_normals == true)
//	{
//		generateVertexNormals();
//	}
//
//	return true;
//}

//void Smoother::ClearAll()
//{
//	tr.clear();
//	vertices.clear();
//	vertex_to_triangle_indices.clear();
//	vertex_to_vertex_indices.clear();
//	vertex_normals.clear();
//	triangle_normals.clear();
//	triangles.clear();
//}

//void Smoother::generateTriangleNormals()
//{
//	if (marching_cube != nullptr)
//	{
//		list<Triangle>& triangles = marching_cube->GetTriangleList();
//
//		if (triangles.size() == 0)
//			return;
//
//		for (list<Triangle>::iterator triangle = triangles.begin(); triangle != triangles.end(); ++triangle)
//		{
//			Vertex v1 = triangle->v[1] - triangle->v[0];
//			Vertex v2 = triangle->v[2] - triangle->v[0];
//			Vertex normal = v1.cross(v2);
//
//			triangle->normal = normal;
//			triangle->normal.Normalize();
//		}
//	}
//
//	else
//	{
//		cout << "'MarchingCube' class object is empty!";
//	}
//}

//void Smoother::generateVertexNormals()
//{
//	if (tr.size() == 0 || vertices.size() == 0)
//		return;
//
//	vertex_normals.clear();
//	vertex_normals.resize(vertices.size());
//
//	for (unsigned int i = 0; i < tr.size(); ++i)
//	{
//		Vertex v1 = vertices[tr[i].vertex_indices[1]] - vertices[tr[i].vertex_indices[0]];
//		Vertex v2 = vertices[tr[i].vertex_indices[2]] - vertices[tr[i].vertex_indices[0]];
//		Vertex v = v1.cross(v2);
//
//		Normal normal; 
//		normal = v;
//
//		vertex_normals[tr[i].vertex_indices[0]] = vertex_normals[tr[i].vertex_indices[0]] + normal;
//		vertex_normals[tr[i].vertex_indices[1]] = vertex_normals[tr[i].vertex_indices[1]] + normal;
//		vertex_normals[tr[i].vertex_indices[2]] = vertex_normals[tr[i].vertex_indices[2]] + normal;
//
//		for (unsigned int i = 0; i < vertex_normals.size(); ++i)
//			vertex_normals[i].Normalize();
//	}
//}

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
	if (marching_cube != nullptr)
	{
		vector<Vertex>& vertices = marching_cube->GetUniqueVertices();
		vector<vector<unsigned int>>& vertex_to_vertex_indices = marching_cube->GetVertexListToVertexIndices();

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

//void Smoother::regenerateTriangleNormalsIfExists()
//{
//	if (marching_cube->GetTriangleCount() > 0)
//	{
//		generateTriangleNormals();
//	}
//}

//void Smoother::regenerateVerticesNormalsIfExists()
//{
//	if (vertex_normals.size() > 0)
//	{
//		generateVertexNormals();
//	}
//}

void Smoother::rebuildMesh()
{
	if (marching_cube != nullptr)
	{
		vector<Indexed_Triangle>& tr = marching_cube->GetTrianglesWithIndexedVertices();
		vector<Vertex>& vertices = marching_cube->GetUniqueVertices();
		list<Triangle>& triangles = marching_cube->GetTriangleList();

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
		cout << "'MarchingCube' class object is empty!";
	}
}

//list<Triangle>& Smoother::GetSmoothedMesh()
//{
//	return this->triangles;
//}