#include "filter.h"
#include "Recorder.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <vector>
#include <array>
#include <fstream>
#include <list>

using namespace std;
using namespace Service::Image;
using namespace Service::Saving;

Filter::Filter(short** PixelsData, short rows, short columns)
{
	this->pixelsData = PixelsData;
	this->rows_image = rows;
	this->columns_image = columns;
}

void Filter::GaussianFilter()
{
	//Storage convolution matrix
	static vector<vector<double>> kernel = GetGaussianKernel();
	static short kernelSize = kernel.size();

	//Initialize new rows and columns values 
	Uint16 new_columns_image = this->columns_image + 2 * (kernelSize / 2);
	Uint16 new_rows_image = this->rows_image + 2 * (kernelSize / 2);

	//Create a new array for apply Gaussian filter
	short** NewPixelsData = new short*[new_rows_image];

	//Form window expended
	for (size_t i = 0; i < new_rows_image; ++i)
	{
		NewPixelsData[i] = new short[new_columns_image];
	}

	//Getting full window 
	NewPixelsData = FillWindow(NewPixelsData, pixelsData, kernelSize);

	//New pixel values will be put here
	short sum = 0;

	//Apply Gaussian filter for image
	for (size_t k = 0; k < rows_image; ++k)
	{
		for (size_t l = 0; l < columns_image; ++l)
		{
			for (size_t i = 0; i < kernelSize; ++i)
			{
				for (size_t j = 0; j < kernelSize; ++j)
				{
					sum += (short)(kernel[i][j] * NewPixelsData[j + k][i + l]);
				}
			}
			pixelsData[k][l] = sum;
			sum = 0;
		}
	}

	for (size_t i = 0; i < new_rows_image; ++i)
	{
		delete[] NewPixelsData[i];
	}
	delete[] NewPixelsData;
}

void Filter::MedianFilter()
{
	//Set and check size of convolution matrix
	static short kernelSize = KernelSizeControlling();

	//Initialize new row and column values
	Uint16 new_columns_image = this->columns_image + 2 * (kernelSize / 2);
	Uint16 new_rows_image = this->rows_image + 2 * (kernelSize / 2);

	//Create a new array for apply Gaussian filter
	short** NewPixelsData = new short*[new_rows_image];

	//Form window expended
	for (size_t i = 0; i < new_rows_image; ++i)
	{
		NewPixelsData[i] = new short[new_columns_image];
	}

	//Getting full window  
	NewPixelsData = FillWindow(NewPixelsData, pixelsData, kernelSize);

	//Create a buffer for sorting array
	short buffer_size = kernelSize*kernelSize;
	short *buffer  = new short[buffer_size];

	//Sorting array and apply median filter for input image
	for (size_t k = 0; k < rows_image; ++k)
	{
		for (size_t l = 0; l < columns_image; ++l)
		{
			for (size_t i = 0; i < kernelSize; ++i)
			{
				for (size_t j = 0; j < kernelSize; ++j)
				{
					buffer[i*kernelSize + j] = NewPixelsData[i + k][j + l];
				}
			}
			pixelsData[k][l] = getMedianValue(buffer, buffer_size, 0, buffer_size - 1);
		}
	}

	for (size_t i = 0; i < new_rows_image; ++i)
	{
		delete[] NewPixelsData[i];
	}
	delete[] NewPixelsData;
}

//This function apply mean filter for input image
void Filter::MeanFilter()
{
	//Set and check size of convolution matrix
	static short kernelSize = KernelSizeControlling();

	//Initialize new row and column values
	Uint16 new_columns_image = this->columns_image + 2 * (kernelSize / 2);
	Uint16 new_rows_image = this->rows_image + 2 * (kernelSize / 2);

	//Create a new array for apply Gaussian filter
	short** NewPixelsData = new short*[new_rows_image];

	//Form window expended
	for (size_t i = 0; i < new_rows_image; ++i)
	{
		NewPixelsData[i] = new short[new_columns_image];
	}

	//Getting full window  
	NewPixelsData = FillWindow(NewPixelsData, pixelsData, kernelSize);

	//Initialize matrix element of mean filter
	static double matrix_element = 1.0 / (kernelSize*kernelSize);

	//New pixel values will be put here
	short sum = 0;

	//Apply mean filter for image
	for (size_t k = 0; k < rows_image; ++k)
	{
		for (size_t l = 0; l < columns_image; ++l)
		{
			for (size_t i = 0; i < kernelSize; ++i)
			{
				for (size_t j = 0; j < kernelSize; ++j)
				{
					sum += (short)(matrix_element * NewPixelsData[j + k][i + l]);
				}
			}
			pixelsData[k][l] = sum;
			sum = 0;
		}
	}

	for (size_t i = 0; i < new_rows_image; ++i)
	{
		delete[] NewPixelsData[i];
	}
	delete[] NewPixelsData;
}

void Filter::ErosionFilter()
{
	//Set and check size of convolution matrix
	static short kernelSize = KernelSizeControlling();

	//This matrix will contain pixels area
	short* BinaryMatrix = new short[kernelSize*kernelSize];

	//To storage pixels area in series
	vector<short> buffer(0);

	//Initialize new row and column values
	Uint16 new_columns_image = this->columns_image + 2 * (kernelSize / 2);
	Uint16 new_rows_image = this->rows_image + 2 * (kernelSize / 2);

	//Create a new array for apply Gaussian filter
	short** NewPixelsData = new short*[new_rows_image];

	//Form window expended
	for (size_t i = 0; i < new_rows_image; ++i)
	{
		NewPixelsData[i] = new short[new_columns_image];
	}

	//Getting full window  
	NewPixelsData = FillWindow(NewPixelsData, pixelsData, kernelSize);

	//This algorithm form area of matrix from pixels
	for (size_t k = 0; k < rows_image; ++k)
	{
		for (size_t l = 0; l < columns_image; ++l)
		{

			for (size_t i = 0; i < kernelSize; ++i)
			{
				if (i <= kernelSize / 2)
				{
					for (size_t j = kernelSize / 2 - i; (j >= 0) && (j <= kernelSize / 2 + i); ++j)
					{
						BinaryMatrix[i*kernelSize + j] = NewPixelsData[i + k][j + l];

						buffer.resize(buffer.size() + 1);
						buffer[buffer.size() - 1] = BinaryMatrix[i*kernelSize + j];
					}
				}
				else
				{
					for (size_t j = i - kernelSize / 2; j < kernelSize - (i - kernelSize / 2); ++j)
					{
						BinaryMatrix[i*kernelSize + j] = NewPixelsData[i + k][j + l];

						buffer.resize(buffer.size() + 1);
						buffer[buffer.size() - 1] = BinaryMatrix[i*kernelSize + j];
					}
				}
			}
			pixelsData[k][l] = *min_element(buffer.begin(), buffer.end());
			buffer.resize(0);
		}
	}

	for (size_t i = 0; i < new_rows_image; ++i)
	{
		delete[] NewPixelsData[i];
	}
	delete[] NewPixelsData;
}

void Filter::DilationFilter()
{
	//Set and check size of convolution matrix
	static short kernelSize = KernelSizeControlling();

	//This matrix will contain pixels area
	short* BinaryMatrix = new short[kernelSize*kernelSize];

	//To storage pixels area in series
	vector<short> buffer(0);

	//Initialize new row and column values
	Uint16 new_columns_image = this->columns_image + 2 * (kernelSize / 2);
	Uint16 new_rows_image = this->rows_image + 2 * (kernelSize / 2);

	//Create a new array for apply Gaussian filter
	short** NewPixelsData = new short*[new_rows_image];

	//Form window expended
	for (size_t i = 0; i < new_rows_image; ++i)
	{
		NewPixelsData[i] = new short[new_columns_image];
	}

	//Getting full window  
	NewPixelsData = FillWindow(NewPixelsData, pixelsData, kernelSize);

	//This algorithm form area of matrix from pixels
	for (size_t k = 0; k < rows_image; ++k)
	{
		for (size_t l = 0; l < columns_image; ++l)
		{

			for (size_t i = 0; i < kernelSize; ++i)
			{
				if (i <= kernelSize / 2)
				{
					for (size_t j = kernelSize / 2 - i; (j >= 0) && (j <= kernelSize / 2 + i); ++j)
					{
						BinaryMatrix[i*kernelSize + j] = NewPixelsData[i + k][j + l];

						buffer.resize(buffer.size() + 1);
						buffer[buffer.size() - 1] = BinaryMatrix[i*kernelSize + j];
					}
				}
				else
				{
					for (size_t j = i - kernelSize / 2; j < kernelSize - (i - kernelSize / 2); ++j)
					{
						BinaryMatrix[i*kernelSize + j] = NewPixelsData[i + k][j + l];

						buffer.resize(buffer.size() + 1);
						buffer[buffer.size() - 1] = BinaryMatrix[i*kernelSize + j];
					}
				}
			}
			pixelsData[k][l] = *max_element(buffer.begin(), buffer.end());
			buffer.resize(0);
		}
	}

	for (size_t i = 0; i < new_rows_image; ++i)
	{
		delete[] NewPixelsData[i];
	}
	delete[] NewPixelsData;
}

short** Filter::GetHandledSlice()
{
	return this->pixelsData;
}

double Filter::GetSigmaSquareValue()
{
	double sigma;
	cout << "Enter the sigma value: ";
	cin >> sigma;
	double sigma_square = sigma*sigma;
	return sigma_square;
}

vector<vector<double>> Filter::GetGaussianKernel()
{
	//Setting and checking size of convolution matrix 
	short size = KernelSizeControlling();

	//Create an array for storage convolution matrix
	vector<vector<double>>kernel(size);

	//Initialize element coordinates of convolution matrix and normalization coefficient
	double x = 0, y = 0, sum = 0;

	//Initialize dispersion
	double sigma_square = GetSigmaSquareValue();

	//Filling coordinate values and getting element values of convolution matrix
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			x = -size / 2 + i;
			y = -size / 2 + j;
			kernel[i].resize(size);
			kernel[i][j] = ((1.0 / 2.0*M_PI*sigma_square)*exp(-(x*x + y*y) / 2.0*sigma_square));
			sum += kernel[i][j];
		}
	}
	//Normalizing convolution matrix
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			kernel[i][j] /= sum;
		}
	}
	return kernel;
}

short** Filter::FillWindow(short** NewPixelsData, short** PixelsData, short kernelSize)
{
	
	//Fill left upper square
	for (size_t i = 0; i < kernelSize / 2; ++i)
	{
		for (size_t j = 0; j < kernelSize / 2; ++j)
		{
			NewPixelsData[i][j] = PixelsData[i][j];
		}
	}

	//Fill right upper square
	for (size_t i = 0; i < kernelSize / 2; ++i)
	{
		for (size_t j = (this->columns_image - kernelSize / 2); j < this->columns_image; ++j)
		{
			NewPixelsData[i][j + 2 * (kernelSize / 2)] = PixelsData[i][j];
		}
	}

	//Fill left lower square
	for (size_t i = (this->rows_image - kernelSize / 2); i < this->rows_image; ++i)
	{
		for (size_t j = 0; j < kernelSize / 2; ++j)
		{
			NewPixelsData[i + 2 * (kernelSize / 2)][j] = PixelsData[i][j];
		}
	}

	//Fill right lower square
	for (size_t i = (this->rows_image - kernelSize / 2); i < this->rows_image; ++i)
	{
		for (size_t j = (this->columns_image - kernelSize / 2); j < this->columns_image; ++j)
		{
			NewPixelsData[i + 2 * (kernelSize / 2)][j + 2 * (kernelSize / 2)] = PixelsData[i][j];
		}
	}

	//Fill field from left upper to right upper squares
	for (size_t i = 0; i < kernelSize / 2; ++i)
	{
		for (size_t j = 0; j < this->columns_image; ++j)
		{
			NewPixelsData[i][j + kernelSize / 2] = PixelsData[i][j];
		}
	}

	//Fill field from left lower to right lower squares
	for (size_t i = (this->rows_image - kernelSize / 2); i < this->rows_image; ++i)
	{
		for (size_t j = 0; j < this->columns_image; ++j)
		{
			NewPixelsData[i + 2 * (kernelSize / 2)][j + kernelSize / 2] = PixelsData[i][j];
		}
	}

	//Fill field from left upper to left lower squares 
	for (size_t i = 0; i < this->rows_image; ++i)
	{
		for (size_t j = 0; j < kernelSize / 2; ++j)
		{
			NewPixelsData[i + kernelSize / 2][j] = PixelsData[i][j];
		}
	}

	//Fill field from right upper to right lower squares  Заполняем границы от правого верхнего до правого нижнего квадрата
	for (size_t i = 0; i < this->rows_image; ++i)
	{
		for (size_t j = (this->columns_image - kernelSize / 2); j < this->columns_image; ++j)
		{
			NewPixelsData[i + kernelSize / 2][j + 2 * (kernelSize / 2)] = PixelsData[i][j];
		}
	}

	//Fill center
	for (size_t i = 0; i < this->rows_image; ++i)
	{
		for (size_t j = 0; j < this->columns_image; ++j)
		{
			NewPixelsData[i + kernelSize / 2][j + kernelSize / 2] = PixelsData[i][j];
		}
	}

	return NewPixelsData;
}

short Filter::KernelSizeControlling()
{
	//Enter the size of convolution matrix
	short size_of_kernel;
	cout << "Enter the size of kernel: ";
	cin >> size_of_kernel;

	//Check odd and non-negative
	if (size_of_kernel % 2 == 0 || size_of_kernel < 0)
	{
		throw new exception("Size is wrong!");
	}

	return size_of_kernel;
}

void Filter::WriteFile(string fileName)
{
	Recodrer::recordSliceToBinaryFile(pixelsData, rows_image, columns_image, fileName);
}
