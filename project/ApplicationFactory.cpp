#include "ApplicationFactory.h"

using namespace Service;

Uint16 ApplicationFactory::rows;
Uint16 ApplicationFactory::columns;
int ApplicationFactory::image_count;
Float64 ApplicationFactory::x_pixelSpacing;
Float64 ApplicationFactory::y_pixelSpacing;
Float64 ApplicationFactory::sliceSpacing;
Float64 ApplicationFactory::x_imagePosition;
Float64 ApplicationFactory::y_imagePosition;
Float64 ApplicationFactory::z_imagePosition;
short*** ApplicationFactory::voxels;

void ApplicationFactory::Initializer(char *argv[])
{
	Float64 sliceLocation1;
	Float64 sliceLocation2;

	//Getting count file in current directory
	image_count = count_if(directory_iterator(argv[1]), directory_iterator(), static_cast<bool(*)(const path&)>(is_regular_file));
	voxels = new short **[image_count];

	std::cout << "Images was found: " << image_count << "\n";

	short current = 0;
	for (recursive_directory_iterator it(argv[1]), end; it != end; it++)
	{
		DcmFileFormat fileformat;
		string name_of_file;

		name_of_file = it->path().string();
		OFCondition status = fileformat.loadFile(name_of_file.c_str());

		if (current == 0)
		{
			if (status.good())
			{
				//Getting rows data
				if (fileformat.getDataset()->findAndGetUint16(DCM_Rows, rows).good())
				{
					cout << "Rows: " << rows << endl;
				}
				//Getting columns data
				if (fileformat.getDataset()->findAndGetUint16(DCM_Columns, columns).good())
				{
					cout << "Columns: " << columns << endl;
				}
				//Getting x pixels spacing
				if (fileformat.getDataset()->findAndGetFloat64(DCM_PixelSpacing, x_pixelSpacing).good())
				{
					cout << "Pixels spacing, X: " << x_pixelSpacing << "\n";
				}
				//Getting y pixels spacing
				if (fileformat.getDataset()->findAndGetFloat64(DCM_PixelSpacing, y_pixelSpacing, 1).good())
				{
					cout << "Pixels spacing, Y: " << y_pixelSpacing << "\n";
				}
				//Getting location for the first slice
				if (fileformat.getDataset()->findAndGetFloat64(DCM_SliceLocation, sliceLocation1).good()) { }

				//Getting x coordinate for first pixel
				if (fileformat.getDataset()->findAndGetFloat64(DCM_ImagePositionPatient, x_imagePosition).good())
				{
					cout << "Image position, X: " << x_imagePosition << "\n";
				}
				//Getting y coordinate for first pixel
				if (fileformat.getDataset()->findAndGetFloat64(DCM_ImagePositionPatient, y_imagePosition, 1).good())
				{
					cout << "Image position, Y: " << y_imagePosition << "\n";
				}
				//Getting z coordinate for first pixel
				if (fileformat.getDataset()->findAndGetFloat64(DCM_ImagePositionPatient, z_imagePosition).good(), 2)
				{
					cout << "Image position, Z: " << z_imagePosition << "\n";
				}
			}
		}

		if (current == 1)
		{
			//Getting location for the second slice
			if (fileformat.getDataset()->findAndGetFloat64(DCM_SliceLocation, sliceLocation2).good())
			{
				sliceSpacing = fabs(sliceLocation1 - sliceLocation2);
				cout << "Slice spacing: " << sliceLocation2 << "\n";
			}
		}

		unsigned long numByte = 0;
		DicomImage *img = new DicomImage(name_of_file.c_str());

		if (img->getStatus() == EIS_Normal)
		{
			const DiPixel *inter = img->getInterData();

			if (inter != NULL)
			{
				numByte = inter->getCount();
				short *raw_pixel_data = (short *)inter->getData();

				if (raw_pixel_data == nullptr)
				{
					cout << "Couldn't acces pixel data!\n";
					exit(1);
				}

				vector<short> pixel_data(
					raw_pixel_data,
					raw_pixel_data + rows * columns);

				if (current < image_count)
				{
					voxels[current] = new short *[rows];

					for (int j = 0; j < rows; ++j)
					{
						voxels[current][j] = new short[columns];

						for (int i = 0; i < columns; ++i)
						{
							voxels[current][j][i] = pixel_data[j*columns + i];
						}
					}
				}

				current++;
			}
		}
		delete img;
	}
}

void ApplicationFactory::clear()
{
	for (int k = 0; k < image_count; ++k)
	{
		for (int j = 0; j < rows; ++j)
		{
			delete[] voxels[k][j];
		}
		delete[] voxels[k];
	}
	delete[] voxels;
}

short*** ApplicationFactory::getImages()
{
	return voxels;
}