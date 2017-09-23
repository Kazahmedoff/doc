#include <string>
#include <conio.h>
#include <thread>

#include "ApplicationFactory.h"
#include "filter.h"
#include "MarchingCube.h"
#include "Smoother.h"
#include "Decimator.h"
#include "Recorder.h"

using namespace std;
using namespace Service;
using namespace Service::Imaging;
using namespace Service::Modeling;
using namespace Service::Smoothing;
using namespace Service::Decimation;
using namespace Service::Saving;

int main(int argc, char *argv[])
{
	//Load tomography slices
	ApplicationFactory::Initializer(argv);

	ImageCollection *collection = ApplicationFactory::GetImageCollection();

	Filter *filter = new Filter(collection);
	filter->MeanFilter();

	short iso_surface = 100;
	string fileName = "D:/Study/Kursach/Project/Models/Test.stl";

	//Building model
	MarchingCube cube(collection, true);
	cube.march(iso_surface);

	//Writing model to binary stl file
	list<Triangle>& triangles = cube.GetTriangleList();
	Recodrer::WriteModelToBinarySTL(triangles, fileName);

	//cube.RecordToBinarySTL(fileName);

	//list<Triangle>& triangles = cube.GetTriangleList();
	//Smoother smoother(triangles);

	////Using Taubin smooth algorithm for model with Fujiwara operator
	//smoother.TaubinSmooth(0.55f, -0.6f, 20);
	//string fileName1 = "D:/Study/Kursach/Project/Models/TestSmoothed33.stl"; 
	//smoother.RecordToBinarySTL(fileName1);

	//smoother.ClearAll();
	cube.ClearTriangleList();
	collection->Clear();
	ApplicationFactory::Clear();

	//collection->Clear();
	//ApplicationFactory::Clear();

	////Getting built triangle model
	//list<Triangle> triangles = cube.getTriangleList();
	//Smoother smoother(triangles);

	cout << "Working have been finished!\n";
	getch();
	return 0;
}
