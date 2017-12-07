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

			Triangle() { }
			Triangle(Vertex v[])
			{
				this->v[0] = v[0];
				this->v[1] = v[1];
				this->v[2] = v[2];

				Normal normal(this->v);
				this->normal = normal;
			}

			Triangle(Vertex v1, Vertex v2, Vertex v3)
			{
				this->v[0] = v1;
				this->v[1] = v2;
				this->v[2] = v3;

				Normal normal(this->v);
				this->normal = normal;
			}
		};
	}
}

#endif //ITRIANGLE_H
