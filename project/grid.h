#pragma once

#include "Vertex.h"

namespace Service {
	namespace Model {

		class GridCell
		{
		public:
			Vertex vertex[8];
			short value[8];

			GridCell();
			GridCell(Vertex[8], short[8]);
		};
	}
}