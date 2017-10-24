#include "MarchingCube.h"

#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <map>
#include <algorithm>
#include <functional>

using namespace std;
using namespace Service::Modeling;

MarchingCube::MarchingCube()
{ }

MarchingCube::MarchingCube(Service::ImageCollection* collection) 
{
	this->collection = collection;
}

MarchingCube::MarchingCube(Service::ImageCollection* collection, bool standartMC) 
{
	this->collection = collection;
	this->standartMC = standartMC;
}

void MarchingCube::March(short iso_surface) 
{
	if (collection->SegmentationMark != Imaging::Binary)
	{
		short count = collection->GetCount();
		short rows = collection->GetImages()[0].Rows;
		short columns = collection->GetImages()[0].Columns;

		cout << "Building model..." << "\n";
		Builder builder(collection, iso_surface, standartMC);

		for (int k = 0; k < count - 1; ++k) {
			for (int j = 0; j < rows - 4; j += 4) {
				for (int i = 0; i < columns - 4; i += 4) {

					if (builder.Build(i, j, k))
						triangles.splice(triangles.end(), builder.getTriangles());

					else
						continue;
				}
			}
		}
		RemoveBadTriangles();
		setData();

		cout << "Model have been built!" << "\n";
	}

	else
	{
		cout << "If you apply this function for segmented images, you will have to call without arguments one. The program was call it!";
		March();
	}
}

void MarchingCube::March()
{
	if (collection->SegmentationMark == Imaging::Binary)
	{
		short count = collection->GetCount();
		short rows = collection->GetImages()[0].Rows;
		short columns = collection->GetImages()[0].Columns;

		cout << "Building model..." << "\n";
		Builder builder(collection, standartMC);

		for (int k = 0; k < count - 1; ++k) {
			for (int j = 0; j < rows - 4; j += 4) {
				for (int i = 0; i < columns - 4; i += 4) {

					if (builder.Build(i, j, k))
						triangles.splice(triangles.end(), builder.getTriangles());

					else
						continue;
				}
			}
		}
		RemoveBadTriangles();
		setData();

		cout << "Model have been built!" << "\n";
	}

	else
	{
		cout << "If you apply this function for non-segmented images, you will have to call with iso_surface argument one!";
	}
}

void MarchingCube::RemoveBadTriangles()
{
	//Remove triangles as line
	unsigned int deleted = 0;
	for (list<Triangle>::iterator triangle = triangles.begin(); triangle != triangles.end(); )
	{
		if ((triangle->v[0] == triangle->v[1]) || (triangle->v[0] == triangle->v[2]) || (triangle->v[1] == triangle->v[2]))
		{
			triangle = triangles.erase(triangle);
			deleted++;
		}

		else
		 ++triangle;
	}
	cout << "Crack triangles have been deleted as line: " << deleted << "\n";
	//----------------------------------------------------------------------
}

void MarchingCube::rebuildMesh()
{
	triangles.clear();
	triangles.resize(tr.size());

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

void MarchingCube::setData()
{
	set<Indexed_Vertex> vertex_set;
	unsigned int tri_index = 0;

	vertices.clear();
	vertex_to_triangle_indices.clear();
	vertex_to_vertex_indices.clear();
	tr.resize(triangles.size());

	for (Triangle triangle : triangles)
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
				{
					vertex_to_vertex_indices_set.insert(tr[tri_index].vertex_indices[k]);
				}
			}
		}
		for (set<unsigned int>::const_iterator iter = vertex_to_vertex_indices_set.begin(); iter != vertex_to_vertex_indices_set.end(); ++iter)
			vertex_to_vertex_indices[i].push_back(*iter);
	}
}

void MarchingCube::ClearTriangleList()
{
	if(triangles.size() != 0)
		triangles.clear();
}

list <Triangle>& MarchingCube::GetTriangleList()
{
	return triangles;
}

vector<Indexed_Triangle>& MarchingCube::GetTrianglesWithIndexedVertices()
{
	return tr;
}

vector<Vertex>& MarchingCube::GetUniqueVertices()
{
	return vertices;
}

vector<vector<unsigned int>>& MarchingCube::GetVertexListToTriangleIndices()
{
	return vertex_to_triangle_indices;
}

vector<vector<unsigned int>>& MarchingCube::GetVertexListToVertexIndices()
{
	return vertex_to_vertex_indices;
}

unsigned int MarchingCube::GetTriangleCount()
{
	return triangles.size();
}