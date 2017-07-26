#pragma once

#include "Vertex.h"

namespace Service {
	namespace Modeling {

		class GridCell
		{
		public:
			Vertex additional_vertex;
			Vertex vertex[8];
			float value[8];
			bool nodeParity[8];

			GridCell();
			GridCell(Vertex[8], short[8]);
		};
	}
}