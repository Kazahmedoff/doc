#pragma once

#pragma once

#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmimgle/dcmimage.h"
#include <vector>

using namespace std;

namespace Service
{
	namespace Image
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
			short** GetHandledSlice();

			//Write in a binary file
			void WriteFile(string);

			//Additional functions
		private:
			//This function get dispersion value
			double GetSigmaSquareValue();

			//This finction build convolution matrix
			vector<vector<double>> GetGaussianKernel();

			//This fuction fill window
			short** FillWindow(short**, short**, short);

			//This function check size of matrix convolution
			short KernelSizeControlling();

		protected:
			short** pixelsData;
			Uint16 rows_image;
			Uint16 columns_image;
		};
	}
}
