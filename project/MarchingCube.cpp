#include "MarchingCube.h"

using namespace std;
using namespace Service::Modeling;
using namespace Service::Imaging;

MarchingCube::MarchingCube()
{ }

MarchingCube::MarchingCube(Service::ImageCollection* collection) 
{
	this->collection = collection;
}

MarchingCube::MarchingCube(Service::ImageCollection* collection, bool standartMC) 
{
	this->collection = collection;
	this->standartMC = standartMC;
}

void MarchingCube::March(short iso_surface) 
{
	if (collection->SegmentationMark != Segmentation::SettedThreshold)
	{
		short count = collection->GetCount();
		short rows = collection->GetImages()[0].Rows;
		short columns = collection->GetImages()[0].Columns;

		cout << "Building model..." << "\n";
		Builder builder(collection, iso_surface, standartMC);

		for (int k = 0; k < count - 1; ++k) {
			for (int j = 0; j < rows - 4; j += 4) {
				for (int i = 0; i < columns - 4; i += 4) {

					if (builder.Build(i, j, k))
						triangles.splice(triangles.end(), builder.getTriangles());

					else
						continue;
				}
			}
		}

		cout << "Model have been built!" << "\n";
	}

	else
	{
		cout << "If you apply this function for segmented images, you will have to call without arguments one. The program was call it!";
		March();
	}
}

void MarchingCube::March()
{
	if (collection->SegmentationMark == Segmentation::SettedThreshold)
	{
		short count = collection->GetCount();
		short rows = collection->GetImages()[0].Rows;
		short columns = collection->GetImages()[0].Columns;

		cout << "Building model..." << "\n";
		Builder builder(collection, standartMC);

		for (int k = 0; k < count - 1; ++k) {
			for (int j = 0; j < rows - 4; j += 4) {
				for (int i = 0; i < columns - 4; i += 4) {

					if (builder.Build(i, j, k))
						triangles.splice(triangles.end(), builder.getTriangles());

					else
						continue;
				}
			}
		}

		cout << "Model have been built!" << "\n";
	}

	else
	{
		cout << "If you apply this function for non-segmented images, you will have to call with iso_surface argument one!";
	}
}

list <Triangle>& MarchingCube::GetTriangleList()
{
	return triangles;
}