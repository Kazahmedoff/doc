#include <string>
#include <conio.h>
#include <thread>
#include <map>

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

	short iso_surface = 120;

	Filter *filter = new Filter(collection);
	filter->GaussianFilter();
	filter->SegmentationBySettedThreshold(iso_surface);

	//Building model
	string fileName = "D:/Study/Kursach/Project/Models/Test.stl";
	MarchingCube *cube = new MarchingCube(collection, false);
	cube->March();

	//Writing model to binary stl file
	list<Triangle>& triangles = cube->GetTriangleList();
	Recodrer::WriteModelToBinarySTL(triangles, fileName);

	////Using Taubin smooth algorithm for model with Fujiwara operator
	//Smoother smoother(cube);
	//smoother.TaubinSmooth(0.55f, -0.6f, 15);
	//string fileName1 = "D:/Study/Kursach/Project/Models/TestSmoothed.stl";
	//Recodrer::WriteModelToBinarySTL(triangles, fileName1);

	collection->Clear();
	delete cube;
	ApplicationFactory::Clear();

	cout << "Working have been finished!\n";
	getch();
	return 0;
}
