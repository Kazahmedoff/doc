#include "MarchingCube.h"

using namespace std;
using namespace Service::Modeling;
using namespace Service::Imaging;

MarchingCube::MarchingCube()
{ }

MarchingCube::MarchingCube(Service::ImageCollection* collection, const CellSize size) 
{
	this->collection = collection;

	switch (size)
	{
	case CellSize::One:
		cell_size = 1;
		break;

	case CellSize::Two:
		cell_size = 2;
		break;

	case CellSize::Four:
		cell_size = 4;
		break;

	case CellSize::Eight:
		cell_size = 8;
		break;
	}
}

MarchingCube::MarchingCube(Service::ImageCollection* collection, bool standartMC, const CellSize size)
{
	this->collection = collection;
	this->standartMC = standartMC;

	switch (size)
	{
	case CellSize::One:
		cell_size = 1;
		break;

	case CellSize::Two:
		cell_size = 2;
		break;

	case CellSize::Four:
		cell_size = 4;
		break;

	case CellSize::Eight:
		cell_size = 8;
		break;
	}
}

void MarchingCube::March(short iso_surface) 
{

	if (collection->SegmentationMark != Segmentation::SettedThreshold)
	{
		short count = collection->GetCount();
		short rows = collection->GetImages()[0].Rows;
		short columns = collection->GetImages()[0].Columns;

		cout << "Building model..." << "\n";
		Builder builder(collection, iso_surface, standartMC, cell_size);

		for (int k = 0; k < count - 1; ++k) {
			for (int j = 0; j < rows - cell_size; j += cell_size) {
				for (int i = 0; i < columns - cell_size; i += cell_size) {

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
		cout << "If you apply this function for segmented images, you will have to call without arguments one. The program have called it!";
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
		Builder builder(collection, standartMC, cell_size);

		for (int k = 0; k < count - 1; ++k) {
			for (int j = 0; j < rows - cell_size; j += cell_size) {
				for (int i = 0; i < columns - cell_size; i += cell_size) {

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