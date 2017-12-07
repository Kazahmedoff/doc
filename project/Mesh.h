#pragma once
#include <list>
#include <vector>

#include "IndexedTriangle.h"
#include "Triangle.h"
#include "Edge.h"

using namespace std;

namespace Service {

	namespace Modeling {

		class Mesh
		{
		public:
			Mesh(list<Triangle>&);
			Mesh(string);

			void FixProblemEdges();
			void RemoveBadTriangles();
			void RepairModel();

			void LoadModelFromBinarySTL(const unsigned int buffer_width = 65536);
			void LoadModelFromSTL();
			void Clear();
			void RebuildMeshData();
			list <Triangle>& GetTriangleList();
			vector<Indexed_Triangle>& GetTrianglesWithIndexedVertices();
			vector<Vertex>& GetUniqueVertices();
			vector<vector<unsigned int>>& GetVertexListToTriangleIndices();
			vector<vector<unsigned int>>& GetVertexListToVertexIndices();
			vector<Edge>& GetEdges();

		private:
			template<typename T> void eliminate_duplicates(vector<T>&);
			bool Mesh::merge_vertex_pair(const size_t keeper, const size_t goner);
			void setData();
			string file_path;
			list<Triangle> triangles;
			vector<Indexed_Triangle> tr;
			vector<Vertex> vertices;
			vector<Edge> edges;
			vector<vector<unsigned int>> vertex_to_triangle_indices;
			vector<vector<unsigned int>> vertex_to_vertex_indices;
		};
	}
}
