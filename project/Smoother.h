#pragma once

#ifndef IMESH_H
#define IMESH_H

#include <list>
#include <vector>
#include "Triangle.h"
#include "IndexedTriangle.h"
#include "MarchingCube.h"

using namespace std;
using namespace Service::Modeling;

namespace Service
{
	namespace Smoothing
	{
		class Smoother
		{
		public:
			Smoother(MarchingCube*);
			Smoother();
			/*bool LoadDataFromBinarySTL(string, const bool generate_triangle_normals = true, const bool generate_vertex_normals = false,
				const unsigned int buffer_width = 65536);*/
			void TaubinSmooth(const float lambda, const float mu, const unsigned short iterations);
			//list<Triangle>& GetSmoothedMesh();
			//void ClearAll();

		private:
			MarchingCube* marching_cube;
			//list<Triangle> triangles;
			//vector<Indexed_Triangle> tr;
			//vector<Vertex> vertices;
			//vector<vector<unsigned int>> vertex_to_triangle_indices;
			//vector<vector<unsigned int>> vertex_to_vertex_indices;
			//vector<Normal> triangle_normals;
			//vector<Normal> vertex_normals;

			//void generateTriangleNormals();
			//void generateVertexNormals();
			//void regenerateTriangleNormalsIfExists();
			//void regenerateVerticesNormalsIfExists();
			void rebuildMesh();
			void laplaceSmooth(const float scale);
		};
	}
}

#endif //IMESH_H