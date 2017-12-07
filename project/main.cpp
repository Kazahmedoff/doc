#include <string>
#include <conio.h>
#include <thread>
#include <map>
#include <set>

#include "ApplicationFactory.h"
#include "filter.h"
#include "MarchingCube.h"
#include "Smoother.h"
#include "Decimator.h"
#include "Recorder.h"
#include "Mesh.h"
#include "CellSize.h"

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

	short iso_surface = 100;

	Filter *filter = new Filter(collection);
	filter->GaussianFilter();
	//filter->SegmentationBySettedThreshold(iso_surface);

	//Building model
	string fileName = "D:/Study/Kursach/Project/Models/Test2.stl";
	MarchingCube *cube = new MarchingCube(collection, false, CellSize::Four);
	cube->March(iso_surface);

	//Writing model to binary stl file
	list<Triangle>& triangles = cube->GetTriangleList();

	//Mesh* mesh = new Mesh(triangles);
	//mesh->RemoveBadTriangles();
	//mesh->FixProblemEdges();

	Recodrer::WriteModelToBinarySTL(triangles, fileName);

	////Using Taubin smooth algorithm for model with Fujiwara operator
	//Smoother smoother(mesh);
	//smoother.TaubinSmooth(0.55f, -0.6f, 15);
	//mesh = smoother.GetSmoothedMesh();
	//string fileName1 = "D:/Study/Kursach/Project/Models/TestSmoothed1.stl";
	//Recodrer::WriteModelToBinarySTL(mesh->GetTriangleList(), fileName1);

	collection->Clear();
	delete cube;
	//delete mesh;
	ApplicationFactory::Clear();

	cout << "Working have been finished!\n";
	getch();
	return 0;
}
