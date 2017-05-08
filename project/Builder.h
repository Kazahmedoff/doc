#pragma once

#ifndef MEDICALIMAGEPROCESSING_EDGETABLE_H
#define MEDICALIMAGEPROCESSING_EDGETABLE_H

#include "Triangle.h"
#include "LookUpTable.h"

#include <vector>
#include <list>
#include <iostream>

using namespace std;

namespace Service
{
	namespace Model
	{
		class Builder {

		private:
			static float dx, dy, dz;
			static short iso_surface;
			bool nodeParity[8];
			bool edgeIntersections[12];

			static short getImageValue(short***, Vertex);
			Vertex getIntersection(short***, float, float, float, short);
			static short getVertexIntensity(short***, float, float, float, short);
			static Vertex getVertex(float, float, float, short);
			static void getVertices(short, short *);
			short getNodeCaseNumber();


		public:
			Builder();
			Builder(float, float, float, short);

			void setValues(short***, float, float, float);

			list<Triangle> getTriangles(short***, float, float, float);
		};
	}
}
#endif //MEDICALIMAGEPROCESSING_EDGETABLE_H
