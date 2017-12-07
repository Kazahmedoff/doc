#pragma once

#ifndef INORMAL_H
#define INORMAL_H

#include <math.h>
#include "Vertex.h"

namespace Service {

	namespace Modeling {

		class Normal {

		private:
			float Length()
			{
				return (float)sqrt((Nx * Nx) + (Ny * Ny) + (Nz * Nz));
			}

		public:
			float Nx, Ny, Nz;

			Normal() { }
			Normal(Vertex v[])
			{
				this->Nx = (v[1].y - v[0].y) * (v[2].z - v[0].z) - (v[1].z - v[0].z) * (v[2].y - v[0].y);
				this->Ny = (v[1].z - v[0].z) * (v[2].x - v[0].x) - (v[1].x - v[0].x) * (v[2].z - v[0].z);
				this->Nz = (v[1].x - v[0].x) * (v[2].y - v[0].y) - (v[1].y - v[0].y) * (v[2].x - v[0].x);
			}

			Normal(float values[])
			{
				this->Nx = values[0];
				this->Ny = values[1];
				this->Nz = values[2];
			}

			void Normalize()
			{
				float length = Length();

				this->Nx = this->Nx / length;
				this->Ny = this->Ny / length;
				this->Nz = this->Nz / length;
			}

			//Inline operators--------------------------------
			inline Normal& operator=(const Vertex &right)
			{
				this->Nx = right.x;
				this->Ny = right.y;
				this->Nz = right.z;

				return *this;
			}

			inline Normal operator+(const Normal &right) const
			{
				Normal n;

				n.Nx = Nx + right.Nx;
				n.Ny = Ny + right.Ny;
				n.Nz = Nz + right.Nz;

				return n;
			}

			inline Normal& operator=(const Normal &right)
			{
				this->Nx = right.Nx;
				this->Ny = right.Ny;
				this->Nz = right.Nz;

				return *this;
			}
		};
	}
}

#endif //INORMAL_H