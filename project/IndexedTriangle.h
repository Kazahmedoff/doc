#pragma once

namespace Service
{
	namespace Modeling
	{
		class Indexed_Triangle
		{
		public:
			unsigned int vertex_indices[3];

			inline bool Indexed_Triangle::Exist(const unsigned int index) const
			{
				for (short i = 0; i < 3; ++i)
				{
					if (vertex_indices[i] == index)
						return true;
				}
				return false;
			}

			inline bool Indexed_Triangle::operator==(const Indexed_Triangle &right) const
			{
				if (right.vertex_indices[0] == vertex_indices[0] && right.vertex_indices[1] == vertex_indices[1] && right.vertex_indices[2] == vertex_indices[2])
					return true;

				return false;
			}
		};
	}
}
