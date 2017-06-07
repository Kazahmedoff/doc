#pragma once

#ifndef IVERTEX_H
#define IVERTEX_H

#include <math.h>

namespace Service
{
	namespace Modeling
	{
		class Vertex {
		public:
			float x, y, z;

			Vertex();
			Vertex(float, float, float);

			//Inline operators
			inline Vertex Vertex::operator-(const Vertex &right) const
			{
				Vertex v;

				v.x = x - right.x;
				v.y = y - right.y;
				v.z = z - right.z;

				return v;
			}

			inline Vertex Vertex::operator+(const Vertex &right) const
			{
				Vertex v;

				v.x = x + right.x;
				v.y = y + right.y;
				v.z = z + right.z;

				return v;
			}

			inline Vertex& Vertex::operator+=(const Vertex &right)
			{
				x += right.x;
				y += right.y;
				z += right.z;

				return *this;
			}

			inline Vertex& Vertex::operator-=(const Vertex &right)
			{
				x -= right.x;
				y -= right.y;
				z -= right.z;

				return *this;
			}

			inline Vertex Vertex::operator*(const float &right) const
			{
				Vertex v;

				v.x = x * right;
				v.y = y * right;
				v.z = z * right;

				return v;
			}

			inline Vertex Vertex::operator/(const float &right) const
			{
				Vertex v;

				v.x = x / right;
				v.y = y / right;
				v.z = z / right;

				return v;
			}

			inline Vertex& Vertex::operator=(const Vertex &right)
			{
				x = right.x;
				y = right.y;
				z = right.z;

				return *this;
			}

			inline bool Vertex::operator==(const Vertex &right) const
			{
				if (right.x == x && right.y == y && right.z == z)
					return true;

				return false;
			}

			inline Vertex Vertex::cross(const Vertex &right) const
			{
				Vertex v;
			
				v.x = y*right.z - z*right.y;
				v.y = z*right.x - x*right.z;
				v.z = x*right.y - y*right.x;
			
				return v;
			}

			inline float Vertex::length() const
			{
				float lenght;
				lenght = sqrt(x*x + y*y + z*z);

				return lenght;
			}

			/*inline bool operator==(const Vertex &right) const;

			inline bool operator<(const Vertex &right) const;

			inline bool operator>(const Vertex &right) const;*/
		};
	}
}

#endif //IVERTEX_H
