#pragma once

//dcmtk library
#include "dcmtk/dcmdata/dcpxitem.h"
#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmimage/diregist.h"
#include "dcmtk/dcmimgle/dcmimage.h"
#include <boost/filesystem.hpp>
#include <boost/lambda/bind.hpp>

using namespace std;
using namespace boost::filesystem;
using namespace boost::lambda;

namespace Service
{
	class ApplicationFactory
	{
	public:
		static void Initializer(char *argv[]);
		static short*** getImages();
		static void clear();

		static Uint16 rows;
		static Uint16 columns;
		static int image_count;
		static Float64 x_pixelSpacing;
		static Float64 y_pixelSpacing;
		static Float64 sliceSpacing;
		static Float64 x_imagePosition;
		static Float64 y_imagePosition;
		static Float64 z_imagePosition;

	private:
		static short*** voxels;

	};
}