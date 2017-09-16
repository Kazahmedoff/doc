#include <string>
#include <conio.h>
#include <thread>

#include "ApplicationFactory.h"

#include "filter.h"

#include "MarchingCube.h"

#include "Smoother.h"

using namespace std;
using namespace Service;
using namespace Service::Imaging;
using namespace Service::Modeling;
using namespace Service::Smoothing;

int main(int argc, char *argv[])
{
	//Load tomography slices
	ApplicationFactory::Initializer(argv);

	ImageCollection *collection = ApplicationFactory::GetImageCollection();

	Filter *filter = new Filter(collection);
	filter->GaussianFilter();

	short iso_surface = 200;
	string fileName = "D:/Study/Kursach/Project/Models/Test33.stl";

	//Building model
	MarchingCube cube(collection, false);
	cube.march(iso_surface);

	//Writing model to binary stl file
	cube.RecordToBinarySTL(fileName);

	list<Triangle>& triangles = cube.GetTriangleList();
	triangles.clear();

	collection->Clear();
	ApplicationFactory::Clear();

	//collection->Clear();
	//ApplicationFactory::Clear();

	////Getting built triangle model
	//list<Triangle> triangles = cube.getTriangleList();

	string fileName1 = "D:/Study/Kursach/Project/Models/TestSmoothed33.stl"; 
	Smoother smoother(triangles);

	////Using Taubin smooth algorithm for model with Fujiwara operator
	smoother.TaubinSmooth(0.55f, -0.6f, 20);
	smoother.RecordToBinarySTL(fileName1);

	cout << "Working have been finished!\n";
	getch();
	return 0;
}
