#pragma once

#pragma once

#include "filter.h"

namespace Service
{
	namespace Imaging
	{
		class MorfologicalFilter : public Filter
		{
		public:
			//Call constructor CreateFilters class
			MorfologicalFilter(short** PixelsData, short rows, short columns) : Filter(PixelsData, rows, columns) { }

			//This function apply open operation for input image
			void OpenFunction();

			//This function apply close operation for input image
			void CloseFunction();

			//This function apply close and open operation for input image successively
			void OpenCloseFunction();

			//This function apply open and close operation for input image successively
			void CloseOpenFunction();
		};
	}
}