#pragma once
#include <list>
#include <vector>

#include "IndexedTriangle.h"
#include "Triangle.h"

using namespace std;

namespace Service {

	namespace Modeling {

		class Mesh
		{
		public:
			Mesh(list<Triangle>&);
			void SetData();

			void FixProblemEdges();
			void RemoveBadTriangles();

			void ClearTriangleList();
			unsigned int GetTriangleCount();
			list <Triangle>& GetTriangleList();
			vector<Indexed_Triangle>& GetTrianglesWithIndexedVertices();
			vector<Vertex>& GetUniqueVertices();
			vector<vector<unsigned int>>& GetVertexListToTriangleIndices();
			vector<vector<unsigned int>>& GetVertexListToVertexIndices();

		private:
			void rebuildMesh();
			list<Triangle> triangles;
			vector<Indexed_Triangle> tr;
			vector<Vertex> vertices;
			vector<vector<unsigned int>> vertex_to_triangle_indices;
			vector<vector<unsigned int>> vertex_to_vertex_indices;
		};
	}
}
