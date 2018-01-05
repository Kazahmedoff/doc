#pragma once

#ifndef Triangle_included
#define Triangle_included

#include <list>

#include "Vertex.h"
#include "Normal.h"
#include "Plane.h"

using namespace Service::Modeling;

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

			inline bool Triangle::IsIntersect(const Triangle &triangle) const
			{
				bool is_intersect = false;

				Plane plane_1(this->v[0], this->normal);
				Plane plane_2(triangle.v[0], triangle.normal);

				//d0, d1, d2 are distances from first triangle to second triangle plane
				float d0, d1, d2;

				//Determine IsLies vertices of first trinagle on the second triangle plane
				if (!plane_2.IsLies(this->v[0]) && !plane_2.IsLies(this->v[1]) && !plane_2.IsLies(this->v[2]))
				{
					d0 = plane_2.GetDistance(this->v[0]);
					d1 = plane_2.GetDistance(this->v[1]);
					d2 = plane_2.GetDistance(this->v[2]);

					//If every vertices heve same sign, then first triangle lies on one side of second triangle plane
					//end operation
					if ((d0 > eps && d1 > eps && d2 > eps) || (d0 < -eps && d1 < -eps && d2 < -eps))
						return is_intersect;
				}

				//This operations is same for second triangle
				if (!plane_1.IsLies(triangle.v[0]) && !plane_1.IsLies(triangle.v[1]) && !plane_1.IsLies(triangle.v[2]))
				{
					d0 = plane_1.GetDistance(triangle.v[0]);
					d1 = plane_1.GetDistance(triangle.v[1]);
					d2 = plane_1.GetDistance(triangle.v[2]);

					if ((d0 > eps && d1 > eps && d2 > eps) || (d0 < -eps && d1 < -eps && d2 < -eps))
						return is_intersect;
				}


				return is_intersect;
			}

			inline Triangle& operator=(const Triangle &right)
			{
				this->v[0] = right.v[0];
				this->v[1] = right.v[1];
				this->v[2] = right.v[2];

				this->normal = right.normal;
				this->quality = right.quality;

				return *this;
			}

		private:
			const float eps = std::numeric_limits<float>::epsilon();

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
#endif
