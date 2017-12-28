#pragma once

#ifndef ITRIANGLE_H
#define ITRIANGLE_H

#include "Vertex.h"
#include "Normal.h"

namespace Service {

	namespace Modeling {

		class Triangle {
		public:
			Vertex v[3];
			Normal normal;
			float quality;

			Triangle() { }
			Triangle(Vertex v[])
			{
				this->v[0] = v[0];
				this->v[1] = v[1];
				this->v[2] = v[2];

				set_normal();
				set_quality();
			}

			Triangle(Vertex &v1, Vertex &v2, Vertex &v3)
			{
				this->v[0] = v1;
				this->v[1] = v2;
				this->v[2] = v3;

				set_normal();
				set_quality();
			}

			inline void Triangle::RecalculateTriangleQuality()
			{
				set_quality();
			}

			inline void Triangle::RecalculateTriangleNormal()
			{
				set_normal();
			}

		private:
			inline void Triangle::set_normal()
			{
				Normal normal(this->v);
				this->normal = normal;
			}

			inline void set_quality()
			{
				Vertex edge1, edge2, edge3;
				edge1 = v[1] - v[0];
				edge2 = v[2] - v[0];
				edge3 = v[2] - v[1];

				float a = edge1.length();
				float b = edge2.length();
				float c = edge3.length();
				float p = static_cast<float>((a + b + c) / 2);
				float area = (float)sqrt(p * (p - a) * (p - b) * (p - c));

				quality = (float)(4 * sqrt(3) * area / static_cast<float>(a * a + b * b + c * c));

				if (isnan(quality))
					quality = 0;
			}
		};
	}
}

#endif //ITRIANGLE_H
