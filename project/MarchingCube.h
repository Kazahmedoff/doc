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

		public:
			bool standartMC = false;

			MarchingCube();
			MarchingCube(ImageCollection*);
			MarchingCube(ImageCollection*, bool);

			void March(short);
			void March();
			list <Triangle>& GetTriangleList();
		};
	}
}

#endif //MEDICALIMAGEPROCESSING_MARCHINGCUBE_H
