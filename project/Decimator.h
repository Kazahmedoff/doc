#pragma once

#include <list>
#include "Triangle.h"

namespace Service
{
	namespace Decimation
	{
		class Decimator
		{
		public:
			Decimator(std::list<Service::Modeling::Triangle>&);
			std::list<Service::Modeling::Triangle>& GetDecimatedMesh();

		private:
			std::list<Service::Modeling::Triangle>& triangles;
		};
	}
}
