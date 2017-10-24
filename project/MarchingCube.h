#pragma once

#ifndef MEDICALIMAGEPROCESSING_MARCHINGCUBE_H
#define MEDICALIMAGEPROCESSING_MARCHINGCUBE_H

#include <vector>

#include "Builder.h"
#include "ImageCollection.h"
#include "IndexedTriangle.h"
#include "IndexedVertex.h"

using namespace std;

namespace Service
{
	namespace Modeling
	{

		class MarchingCube {
		private:
			list<Triangle> triangles;
			ImageCollection* collection;
			vector<Indexed_Triangle> tr;
			vector<Vertex> vertices;
			vector<vector<unsigned int>> vertex_to_triangle_indices;
			vector<vector<unsigned int>> vertex_to_vertex_indices;

			void rebuildMesh();
			void setData();

		public:
			bool standartMC = false;

			MarchingCube();
			MarchingCube(ImageCollection*);
			MarchingCube(ImageCollection*, bool);

			void March(short);
			void March();
			void ClearTriangleList();
			void RemoveBadTriangles();
			unsigned int GetTriangleCount();
			list <Triangle>& GetTriangleList();
			vector<Indexed_Triangle>& GetTrianglesWithIndexedVertices();
			vector<Vertex>& GetUniqueVertices();
			vector<vector<unsigned int>>& GetVertexListToTriangleIndices();
			vector<vector<unsigned int>>& GetVertexListToVertexIndices();
		};
	}
}

#endif //MEDICALIMAGEPROCESSING_MARCHINGCUBE_H
