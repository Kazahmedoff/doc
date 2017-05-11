#pragma once

#ifndef MEDICALIMAGEPROCESSING_MARCHINGCUBE_H
#define MEDICALIMAGEPROCESSING_MARCHINGCUBE_H

#include "Builder.h"

using namespace std;

namespace Service
{
	namespace Model
	{

		class MarchingCube {
		private:
			list <Triangle> triangles;
			short*** voxels;
			short sx, sy, sz;
			float dx, dy, dz;

			void fixModel();

		public:
			MarchingCube(short***, short, short, short, float, float, float);
			MarchingCube(string);

			void march(short);
			void recordToBinarySTL(string);
			void recordToPLY(string);
			void recordToSTL(string);
			list <Triangle> getTriangleList();
		};
	}
}

#endif //MEDICALIMAGEPROCESSING_MARCHINGCUBE_H
