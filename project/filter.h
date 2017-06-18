#pragma once

#include <vector>

using namespace std;

namespace Service
{
	namespace Imaging
	{
	
		class Filter
		{
		public:
			//CreateFilters class constructor
			Filter(short **, short, short);

			//This function apply Gaussian filter for input image 
			void GaussianFilter();

			//This function apply median filter for input image
			void MedianFilter();

			//This function apply mean filter for input image
			void MeanFilter();

			//This function apply erosion filter for input image
			void ErosionFilter();

			//This function apply dilation filter for input image
			void DilationFilter();

			//Getting handled image
			short** getHandledSlice();

			//Write in a binary file
			void writeToFile(string);

			//Additional functions
		private:
			//This function get dispersion value
			double setSigmaSquareValue();

			//This finction build convolution matrix
			vector<vector<float>> getGaussianKernel();

			//This function check size of matrix convolution
			short kernelSizeControlling();

			//This function fill window
			void fillWindow(short**, short**, short);

			inline short getMedianValue(short buffer[], short buffer_size, short b, short e)
			{
				short l = b, r = e;
				short piv = buffer[(l + r) / 2];
				while (l <= r)
				{
					while (buffer[l] < piv)
						l++;
					while (buffer[r] > piv)
						r--;
					if (l <= r)
						swap(buffer[l++], buffer[r--]);
				}
				if (b < r)
					getMedianValue(buffer, buffer_size, b, r);
				if (e > l)
					getMedianValue(buffer, buffer_size, l, e);

				return buffer[buffer_size / 2];
			} 

			inline short getMinValue(short buffer[], short buffer_size)
			{
				short min = buffer[0];

				for (short i = 0; i < buffer_size; ++i)
					if (buffer[i] < min)
						min = buffer[i];

				return min;
			}

			inline short getMaxValue(short buffer[], short buffer_size)
			{
				short max = buffer[0];

				for (short i = 0; i < buffer_size; ++i)
					if (buffer[i] > max)
						max = buffer[i];

				return max;
			}

		protected:
			short** pixelsData;
			unsigned short rows_image;
			unsigned short columns_image;
		};
	}
}
