#include "MarchingCube.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
using namespace Service::Modeling;

MarchingCube::MarchingCube(Service::ImageCollection* collection) 
{
	this->collection = collection;
}

MarchingCube::MarchingCube(Service::ImageCollection* collection, bool standartMC) 
{
	this->collection = collection;
	this->standartMC = standartMC;
}

void MarchingCube::march(short iso_surface) 
{
	short count = collection->GetCount();
	short rows = collection->GetImages()[0].Rows;
	short columns = collection->GetImages()[0].Columns;

	cout << "Building model..." << "\n";
	Builder builder(collection, iso_surface, standartMC);

	for (int k = 0; k < count - 1; ++k) {
		for (int j = 0; j < rows - 4; j+=4) {
			for (int i = 0; i < columns - 4; i+=4) {

				if (builder.Build(i, j, k))
					triangles.splice(triangles.end(), builder.getTriangles());

				else
					continue;
			}
		}
	}
	fixModel();
	cout << "Model have been built!" << "\n";
}

void MarchingCube::fixModel()
{
	unsigned int i = 0;

	for (list<Triangle>::iterator it = triangles.begin(); it != triangles.end(); )
	{
		if (((*it).v[0] == (*it).v[1]) || ((*it).v[0] == (*it).v[2]) || ((*it).v[1] == (*it).v[2]))
		{
			it = this->triangles.erase(it);
			i++;
		}

		else
		 ++it;
	}
	cout << "Crack triangles have been deleted: " << i << "\n";
}

void MarchingCube::ClearTriangleList()
{
	if(triangles.size() != 0)
		triangles.clear();
}

list <Triangle>& MarchingCube::GetTriangleList()
{
	return triangles;
}