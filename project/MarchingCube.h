#pragma once

#ifndef MEDICALIMAGEPROCESSING_MARCHINGCUBE_H
#define MEDICALIMAGEPROCESSING_MARCHINGCUBE_H

#include <vector>

#include "Builder.h"
#include "ImageCollection.h"
#include "IndexedTriangle.h"
#include "IndexedVertex.h"
#include "CellSize.h"
#include "Mesh.h"

using namespace std;

namespace Service
{
	namespace Modeling
	{

		class MarchingCube {
		private:
			Mesh *mesh;
			ImageCollection* collection;
			short cell_size;

		public:
			bool standartMC = false;

			MarchingCube();
			MarchingCube(ImageCollection*, const bool standartMC = false, const CellSize size = CellSize::One);

			void March(short);
			void March();
			Mesh* GetMesh();
		};
	}
}

#endif //MEDICALIMAGEPROCESSING_MARCHINGCUBE_H
