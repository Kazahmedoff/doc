#pragma once

#ifndef INORMAL_H
#define INORMAL_H

#include <math.h>
#include "Vertex.h"

namespace Service {

	namespace Modeling {

		class Normal {

		private:
			float Length();

		public:
			float Nx, Ny, Nz;

			Normal(Vertex[]);
			Normal();
			Normal Normalize();

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