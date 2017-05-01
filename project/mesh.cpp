#include <iostream>
#include <fstream>
#include <set>
#include <algorithm>
#include <numeric>

#include "mesh.h"
#include "IndexedVertex.h"

using namespace std;
using namespace Service::Smoothing;

Mesh::Mesh() { }

Mesh::Mesh(list<Triangle> triangles)
{
	this->triangles = triangles;
	this->tr.resize(triangles.size());
	MeshInitializer();
}

void Mesh::MeshInitializer(const bool generate_triangle_normals, const bool generate_vertex_normals)
{
	set<Indexed_Vertex> vertex_set;
	unsigned int tri_index = 0;

	for (Triangle triangle : this->triangles)
	{
		for (unsigned short i = 0; i < 3; ++i)
		{
			Indexed_Vertex v;

			v.x = triangle.v[i].x;
			v.y = triangle.v[i].y;
			v.z = triangle.v[i].z;

			set<Indexed_Vertex>::const_iterator it = vertex_set.find(v);

			if (vertex_set.end() == it)
			{
				v.index = vertices.size();

				vertex_set.insert(v);

				Vertex vertex;
				vertex.x = v.x;
				vertex.y = v.y;
				vertex.z = v.z;
				vertices.push_back(vertex);

				tr[tri_index].vertex_indices[i] = v.index;

				vector<unsigned int> tri_indices;
				tri_indices.push_back(tri_index);
				vertex_to_triangle_indices.push_back(tri_indices);
			}

			else
			{
				tr[tri_index].vertex_indices[i] = it->index;

				vertex_to_triangle_indices[it->index].push_back(tri_index);
			}
		}
		tri_index++;
	}
	vertex_set.clear();

	vertex_to_vertex_indices.resize(vertices.size());

	for (unsigned int i = 0; i < vertex_to_triangle_indices.size(); ++i)
	{
		set<unsigned int> vertex_to_vertex_indices_set;

		for (unsigned int j = 0; j < vertex_to_triangle_indices[i].size(); ++j)
		{
			tri_index = vertex_to_triangle_indices[i][j];

			for (short k = 0; k < 3; ++k)
			{
				if (i != tr[tri_index].vertex_indices[k])
				{ vertex_to_vertex_indices_set.insert(tr[tri_index].vertex_indices[k]); }
			}
		}
		for (set<unsigned int>::const_iterator iter = vertex_to_vertex_indices_set.begin(); iter != vertex_to_vertex_indices_set.end(); ++iter)
			vertex_to_vertex_indices[i].push_back(*iter);
	}

	cout << "Vertices: " << triangles.size() * 3 << " (of which " << vertices.size() << " are unique)" << "\n";
	SortingTriangleIndices();
	SortingVertexIndicesAtTriangles();

	if (generate_triangle_normals == true)
	{
		GenerateTriangleNormals();
	}

	if (generate_vertex_normals == true)
	{
		GenerateVertexNormals();
	}
}

void Mesh::GenerateTriangleNormals()
{
	if (tr.size() == 0)
		return;

	triangle_normals.clear();
	triangle_normals.resize(tr.size());

	for (unsigned int i = 0; i < tr.size(); ++i)
	{
		Vertex v1 = vertices[tr[i].vertex_indices[1]] - vertices[tr[i].vertex_indices[0]];
		Vertex v2 = vertices[tr[i].vertex_indices[2]] - vertices[tr[i].vertex_indices[0]];
		Vertex v = v1.cross(v2);

		triangle_normals[i] = v;
		triangle_normals[i] = triangle_normals[i].Normalize();
	}
}

void Mesh::GenerateVertexNormals()
{
	if (tr.size() == 0 || vertices.size() == 0)
		return;

	vertex_normals.clear();
	vertex_normals.resize(vertices.size());

	for (unsigned int i = 0; i < tr.size(); ++i)
	{
		Vertex v1 = vertices[tr[i].vertex_indices[1]] - vertices[tr[i].vertex_indices[0]];
		Vertex v2 = vertices[tr[i].vertex_indices[2]] - vertices[tr[i].vertex_indices[0]];
		Vertex v = v1.cross(v2);

		Normal normal; 
		normal = v;

		vertex_normals[tr[i].vertex_indices[0]] = vertex_normals[tr[i].vertex_indices[0]] + normal;
		vertex_normals[tr[i].vertex_indices[1]] = vertex_normals[tr[i].vertex_indices[1]] + normal;
		vertex_normals[tr[i].vertex_indices[2]] = vertex_normals[tr[i].vertex_indices[2]] + normal;

		for (unsigned int i = 0; i < vertex_normals.size(); ++i)
		{
			vertex_normals[i] = vertex_normals[i].Normalize();
		}
	}
}

void Mesh::TaubinSmooth(const float lambda, const float mu, const unsigned short iterations)
{
	cout << "Smoothing mesh using Taubin lambda|mu algorithm ";
	cout << "(inverse neighbour count weighting)" << "\n";

	for (unsigned short i = 0; i < iterations; ++i)
	{
		cout << "Iteration " << i + 1 << " of " << iterations << "\n";

		LaplaceSmooth(lambda);
		LaplaceSmooth(mu);
	}

	RegenerateTriangleNormalsIfExists();
	BuildNewMesh();
}

void Mesh::LaplaceSmooth(const float scale)
{
	vector<Vertex> displacements(vertices.size(), Vertex(0, 0, 0));

	for (unsigned int i = 0; i < vertices.size(); ++i)
	{

			if (vertex_to_vertex_indices[i].size() == 0)
				continue;

			vector<float> weights(vertex_to_vertex_indices[i].size(), 0.0f);

			for (unsigned int j = 0; j < vertex_to_vertex_indices[i].size(); ++j)
			{
				unsigned int neighbour_1 = vertex_to_vertex_indices[i][j];
				unsigned int group[] = { i, neighbour_1 };
				sort(group, group + 2);

				vector<unsigned int>common_triangle_indices(3);

				vector<unsigned int>::iterator it = set_intersection(vertex_to_triangle_indices[i].begin(), vertex_to_triangle_indices[i].end(),
					vertex_to_triangle_indices[neighbour_1].begin(), vertex_to_triangle_indices[neighbour_1].end(), common_triangle_indices.begin());

				common_triangle_indices.resize(it - common_triangle_indices.begin());

				if (common_triangle_indices.size() != 2)
					continue;

				for (short k = 0; k < common_triangle_indices.size(); ++k)
				{
					unsigned int tri_index = common_triangle_indices[k];

					vector<unsigned int> buffer(1);
					set_difference(tr[tri_index].vertex_indices, tr[tri_index].vertex_indices + 3, group, group + 2, buffer.begin());
					unsigned int neighbour_2 = buffer[0];

					//Set vectors to triangle sides
					Vertex vect_a = vertices[i] - vertices[neighbour_1];
					Vertex vect_b = vertices[i] - vertices[neighbour_2];
					Vertex vect_c = vertices[neighbour_2] - vertices[neighbour_1];

					//Find side lenghts
					float a = vect_a.length();
					float b = vect_b.length();
					float c = vect_c.length();

					float cos_x = (b * b + c * c - a * a) / (2.0 * b * c);
					float ctg_x = cos_x / sqrt(1 - cos_x * cos_x);

					weights[j] += ctg_x;
				}
			}
			//Find displacement for each vertex
			float sum = accumulate(weights.begin(), weights.end(), 0.0);

			if (sum != 0)
			{
				for (unsigned int j = 0; j < vertex_to_vertex_indices[i].size(); ++j)
				{
					displacements[i] += ((vertices[vertex_to_vertex_indices[i][j]] * weights[j]) - vertices[i]) / sum;
				}
			}
		}

	for (unsigned int i = 0; i < vertices.size(); ++i)
	{
		if (displacements[i] == Vertex(0, 0, 0))
			continue;

		vertices[i] += (displacements[i] * scale);
	}
}

void Mesh::RegenerateTriangleNormalsIfExists()
{
	if (triangle_normals.size() > 0)
	{
		GenerateTriangleNormals();
	}
}

void Mesh::RegenerateVerticesNormalsIfExists()
{
	if (vertex_normals.size() > 0)
	{
		GenerateVertexNormals();
	}
}

void Mesh::SortingVertexIndicesAtTriangles()
{
	for (unsigned int i = 0; i < tr.size(); ++i)
	{
		sort(tr[i].vertex_indices, tr[i].vertex_indices + 3);
	}
}

void Mesh::BuildNewMesh()
{
	triangles.clear();
	triangles.resize(tr.size());

	unsigned int tri_index = 0;
	for (Triangle triangle : triangles)
	{
		triangle.v[0] = vertices[tr[tri_index].vertex_indices[0]];
		triangle.v[1] = vertices[tr[tri_index].vertex_indices[1]];
		triangle.v[2] = vertices[tr[tri_index].vertex_indices[2]];

		triangle.normal = triangle_normals[tri_index];

		tri_index++;
	}
}

list<Triangle> Service::Smoothing::Mesh::getSmoothedMesh()
{
	return this->triangles;;
}

void Mesh::SaveToSTL(string fileName)
{
		ofstream model(fileName.c_str(), ios_base::binary | ios_base::trunc);

		vector<char> buffer(80, '0');
		unsigned int triangle_num = this->triangles.size();

		model.write(reinterpret_cast<const char *>(&buffer[0]), 80);
		model.write(reinterpret_cast<const char *>(&triangle_num), sizeof(triangle_num));
		unsigned short attribute_byte_count = 0;

		for (Triangle triangle : this->triangles)
		{
			float buffer_x = triangle.normal.Nx;
			float buffer_y = triangle.normal.Ny;
			float buffer_z = triangle.normal.Nz;

			model.write(reinterpret_cast<const char *>(&buffer_x), sizeof(buffer_x));
			model.write(reinterpret_cast<const char *>(&buffer_y), sizeof(buffer_y));
			model.write(reinterpret_cast<const char *>(&buffer_z), sizeof(buffer_z));

			for (short i = 0; i < 3; ++i)
			{
				buffer_x = triangle.v[i].x;
				buffer_y = triangle.v[i].y;
				buffer_z = triangle.v[i].z;

				model.write(reinterpret_cast<const char *>(&buffer_x), sizeof(buffer_x));
				model.write(reinterpret_cast<const char *>(&buffer_y), sizeof(buffer_y));
				model.write(reinterpret_cast<const char *>(&buffer_z), sizeof(buffer_z));
			}

			model.write(reinterpret_cast<const char *>(&attribute_byte_count), sizeof(attribute_byte_count));
		}

		model.close();
	}


void Mesh::SortingTriangleIndices()
{
	for (unsigned int i = 0; i < vertex_to_triangle_indices.size(); ++i)
	{
		if (vertex_to_triangle_indices[i].size() == 0)
			continue;

		for (unsigned int j = 0; j < vertex_to_triangle_indices[i].size(); ++j)
		  sort(vertex_to_triangle_indices[i].begin(), vertex_to_triangle_indices[i].end());
	}
}