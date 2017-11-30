#include <set>
#include <map>
#include <functional>
#include <iostream>

#include "Mesh.h"
#include "IndexedVertex.h"

using namespace Service::Modeling;

Mesh::Mesh(list<Triangle> &triangles)
{
	this->triangles = triangles;
}

void Mesh::SetData()
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

void Mesh::RemoveBadTriangles()
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

	//Remove excess triangle from triangle fan
	set<unsigned int, std::greater<int>> removable_triangle;

	for (size_t i = 0; i < vertex_to_vertex_indices.size(); ++i)
	{
		//Insert vertices----->triangles
		multimap<unsigned int, unsigned int> vertex_to_triangles;
		for (size_t j = 0; j < vertex_to_vertex_indices[i].size(); ++j)
		{
			unsigned int vert_index = vertex_to_vertex_indices[i][j];

			for (size_t k = 0; k < vertex_to_triangle_indices[i].size(); ++k)
			{
				unsigned int tri_index = vertex_to_triangle_indices[i][k];

				if (tr[tri_index].Exist(vert_index))
					vertex_to_triangles.insert(pair<unsigned int, unsigned int>(vert_index, tri_index));
			}
		}

		if (vertex_to_triangles.size() != 0)
		{
			//Find all vertices belong a once triangle
			map<unsigned int, unsigned int> vertex_to_triangle;
			for (multimap<unsigned int, unsigned int>::iterator iter = vertex_to_triangles.begin(); iter != vertex_to_triangles.end(); ++iter)
			{
				if (vertex_to_triangles.count(iter->first) == 1)
					vertex_to_triangle.insert(pair<unsigned int, unsigned int>(iter->first, iter->second));
			}

			//Find removable triangles
			if (vertex_to_triangle.size() > 1)
			{
				set<unsigned int> triangle_buffer;
				for (map<unsigned int, unsigned int>::iterator vertex = vertex_to_triangle.begin(); vertex != vertex_to_triangle.end(); ++vertex)
				{
					set<unsigned int>::iterator triangle = triangle_buffer.find(vertex->second);
					if (triangle != triangle_buffer.end())
						removable_triangle.insert(vertex->second);

					else
						triangle_buffer.insert(vertex->second);
				}
			}
		}
	}

	//Remove founded triangles
	for (set<unsigned int>::iterator it = removable_triangle.begin(); it != removable_triangle.end(); ++it)
	{
		list<Triangle>::iterator triangle = triangles.begin();
		advance(triangle, (*it));
		triangles.erase(triangle);
	}

	cout << "Excess triangles was deleted: " << removable_triangle.size() << " from triangle fans" << "\n";
	//---------------------------------------------------------------------------------------------------------

	rebuildMesh();
}

void Mesh::rebuildMesh()
{
	SetData();
}

void Mesh::FixProblemEdges()
{

}

list <Triangle>& Mesh::GetTriangleList()
{
	return triangles;
}

vector<Indexed_Triangle>& Mesh::GetTrianglesWithIndexedVertices()
{
	return tr;
}

vector<Vertex>& Mesh::GetUniqueVertices()
{
	return vertices;
}

vector<vector<unsigned int>>& Mesh::GetVertexListToTriangleIndices()
{
	return vertex_to_triangle_indices;
}

vector<vector<unsigned int>>& Mesh::GetVertexListToVertexIndices()
{
	return vertex_to_vertex_indices;
}

unsigned int Mesh::GetTriangleCount()
{
	return triangles.size();
}

void Mesh::ClearTriangleList()
{
	if (triangles.size() != 0)
		triangles.clear();
}