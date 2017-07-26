#pragma once

#ifndef MEDICALIMAGEPROCESSING_EDGETABLE_H
#define MEDICALIMAGEPROCESSING_EDGETABLE_H

#include "Triangle.h"
#include "LookUpTable.h"
#include "grid.h"

#include <list>
#include <iostream>

using namespace std;

namespace Service
{
	namespace Modeling
	{
		class Builder {

		private:
			float dx, dy, dz;
			short iso_surface;
			bool standartMC;
			GridCell cell;
			short tunnelOrientation = 0;

			Vertex getIntersection(short);
			void getVertices(short, short *);
			short getNodeCaseNumber();
			void addTriangles(list<Triangle> &, const __int8[], short);
			bool testFace(__int8);
			bool modifiedTestInterior(__int8, short, short);
			__int8 interiorAmbiguity(short, __int8);
			__int8 interiorAmbiguityVerification(__int8);
			bool interiorTestCase13();


		public:
			Builder();
			Builder(float, float, float, short, bool);

			bool setValues(short***, short, short, short);
			list<Triangle> getTriangles();
		};
	}
}
#endif //MEDICALIMAGEPROCESSING_EDGETABLE_H
