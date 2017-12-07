#pragma once

#ifndef MEDICALIMAGEPROCESSING_MARCHINGCUBE_H
#define MEDICALIMAGEPROCESSING_MARCHINGCUBE_H

#include <vector>

#include "Builder.h"
#include "ImageCollection.h"
#include "IndexedTriangle.h"
#include "IndexedVertex.h"
#include "CellSize.h"

using namespace std;

namespace Service
{
	namespace Modeling
	{

		class MarchingCube {
		private:
			list<Triangle> triangles;
			ImageCollection* collection;
			short cell_size;

		public:
			bool standartMC = false;

			MarchingCube();
			MarchingCube(ImageCollection*, const CellSize size = CellSize::One);
			MarchingCube(ImageCollection*, bool, const CellSize size = CellSize::One);

			void March(short);
			void March();
			list <Triangle>& GetTriangleList();
		};
	}
}

#endif //MEDICALIMAGEPROCESSING_MARCHINGCUBE_H
