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
	namespace Model
	{
		class Builder {

		private:
			float dx, dy, dz;
			short iso_surface;
			bool nodeParity[8];
			Vertex intersection_pointList[12];
			GridCell cell;

			Vertex getIntersection(short);
			void getVertices(short, short *);
			short getNodeCaseNumber();


		public:
			Builder();
			Builder(float, float, float, short);

			bool setValues(short***, short, short, short);
			list<Triangle> getTriangles();
		};
	}
}
#endif //MEDICALIMAGEPROCESSING_EDGETABLE_H
