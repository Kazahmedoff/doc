#pragma once

#ifndef IMESH_H
#define IMESH_H

#include <list>
#include <vector>
#include "Triangle.h"
#include "IndexedTriangle.h"

using namespace std;
using namespace Service::Model;

namespace Service
{
	namespace Smoothing
	{
		class Mesh
		{
		private:
			list<Triangle> triangles;
			vector<Indexed_Triangle> tr;
			vector<Vertex> vertices;
			vector<vector<unsigned int>> vertex_to_triangle_indices;
			vector<vector<unsigned int>> vertex_to_vertex_indices;
			vector<Normal> triangle_normals;
			vector<Normal> vertex_normals;

			void GenerateTriangleNormals();
			void GenerateVertexNormals();
			void RegenerateTriangleNormalsIfExists();
			void RegenerateVerticesNormalsIfExists();
			//void SortingTriangleIndices();
			//void SortingVertexIndicesAtTriangles();
			void BuildNewMesh();
			void LaplaceSmooth(const float scale);

		public:
			Mesh();
			Mesh(list<Triangle>);

			void MeshInitializer(const bool generate_triangle_normals = true, const bool generate_vertex_normals = false);
			void TaubinSmooth(const float lambda, const float mu, const unsigned short iterations);
			list<Triangle> getSmoothedMesh();
			void SaveToSTL(string);
		};
	}
}

#endif //IMESH_H