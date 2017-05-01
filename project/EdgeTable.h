#pragma once

#ifndef MEDICALIMAGEPROCESSING_EDGETABLE_H
#define MEDICALIMAGEPROCESSING_EDGETABLE_H

#include "Triangle.h"
#include <vector>
#include <list>
#include <iostream>

using namespace std;

namespace Service
{
	namespace Model
	{
		class EdgeTable {

		private:
			static float dx, dy, dz;
			bool nodeParity[8];
			bool edgeIntersections[12];

			static short getImageValue(short***, Vertex);

			Vertex getIntersection(short***, float, float, float, short, short);

			static short getVertexIntensity(short***, float, float, float, short);

			static Vertex getVertex(float, float, float, short);

			static void getVertices(short, short *);

			short getNodeCaseNumber();

		public:
			EdgeTable();
			EdgeTable(float, float, float);

			void setValues(short***, float, float, float, short);

			list<Triangle> getTriangles(short***, float, float, float, short);
		};
	}
}
#endif //MEDICALIMAGEPROCESSING_EDGETABLE_H
