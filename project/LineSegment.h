#pragma once

#include "Vertex.h"

namespace Service
{
	namespace Modeling
	{
		namespace Geometry
		{
			class LineSegment
			{
			public:
				Vertex vertices[2];

				LineSegment() { }
				LineSegment(Vertex &a, Vertex &b)
				{
					float len_a = sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
					float len_b = sqrt(b.x*b.x + b.y*b.y + b.z*b.z);

					if (len_a < len_b)
					{
						vertices[0] = a;
						vertices[1] = b;
					}

					else
					{
						vertices[0] = b;
						vertices[1] = a;
					}
				}

				inline bool LineSegment::IsIntersect(const LineSegment &segment) const
				{


				}
			};
		}
	}
}