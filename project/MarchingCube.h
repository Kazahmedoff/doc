#pragma once

#ifndef MEDICALIMAGEPROCESSING_MARCHINGCUBE_H
#define MEDICALIMAGEPROCESSING_MARCHINGCUBE_H

#include "Builder.h"
#include "ImageCollection.h"

using namespace std;

namespace Service
{
	namespace Modeling
	{

		class MarchingCube {
		private:
			list <Triangle> triangles;
			ImageCollection* collection;

			void fixModel();

		public:
			bool standartMC = false;

			MarchingCube(ImageCollection*);
			MarchingCube(ImageCollection*, bool);

			void march(short);
			void ClearTriangleList();
			list <Triangle>& GetTriangleList();
		};
	}
}

#endif //MEDICALIMAGEPROCESSING_MARCHINGCUBE_H
