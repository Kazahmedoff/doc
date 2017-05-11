#pragma once

#include "Triangle.h"
#include <list>

using namespace std;
using namespace Service::Model;

namespace Service {
	namespace Saving
	{
		class Recodrer
		{
		public:
			static bool recordModelToBinarySTL(list<Triangle>, string);
			static bool recordModelToSTL(list<Triangle>, string);
			static bool recordModelToPLY(list<Triangle>, string);
			static bool recordSliceToBinaryFile(short**, short, short, string);
		};
	}
}