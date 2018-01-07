#pragma once

#include "Vertex.h"
#include "Normal.h"

#include <limits>

namespace Service
{
	namespace Modeling
	{
		namespace Geometry
		{
			class Plane
			{
			private:
				const float eps = std::numeric_limits<float>::epsilon();

			public:
				float A, B, C, D;

				Plane() { }
				Plane(Vertex vertex, Normal normal)
				{
					this->A = normal.Nx;
					this->B = normal.Ny;
					this->C = normal.Nz;

					float x = vertex.x;
					float y = vertex.y;
					float z = vertex.z;

					this->D = -(A*x + B*y + C*z);
				}

				Plane(float A, float B, float C, float D)
				{
					this->A = A;
					this->B = B;
					this->C = C;
					this->D = D;
				}

				inline bool Plane::IsLies(const Vertex &vertex) const
				{
					float distance = GetDistance(vertex);

					if ((distance >= 0 && distance <= eps) || (distance >= -eps && distance <= 0))
						return true;

					return false;
				}

				inline float Plane::GetDistance(const Vertex &vertex) const
				{
					float distance = A*vertex.x + B*vertex.y + C*vertex.z + D;

					return distance;
				}
			};
		}
	}
}