#pragma once

#ifndef IMAGE_H
#define IMAGE_H

namespace Service
{
	class Image
	{
	public:
		Image();
		Image(short, short, short**);
		Image(short, short);

		short Rows;
		short Columns;
		short** Data;

		inline Image& Image::operator=(const Image &right)
		{
			Rows = right.Rows;
			Columns = right.Columns;
			Data = right.Data;

			return *this;
		}
	};
}
#endif //IMAGE_H

