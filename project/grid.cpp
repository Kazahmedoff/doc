#include "grid.h"

using namespace Service::Modeling;

GridCell::GridCell() { }

GridCell::GridCell(Vertex vertex[8], short value[8])
{
	for (short i = 0; i < 8; ++i)
	{
		this->vertex[i] = vertex[i];
		this->value[i] = value[i];
	}
}