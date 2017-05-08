#pragma once

#ifndef ITRIANGLE_H
#define ITRIANGLE_H

#include "Vertex.h"
#include "Normal.h"

namespace Service {

	namespace Model {

		class Triangle {
		public:
			Vertex v[3];
			Normal normal;

			Triangle(Vertex[]);
			Triangle();
		};
	}
}

#endif //ITRIANGLE_H
