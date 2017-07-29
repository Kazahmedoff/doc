#include <string>
#include <conio.h>
#include <thread>

#include "ApplicationFactory.h"

#include "filter.h"
#include "MorfologicalFilter.h"

#include "MarchingCube.h"

#include "Smoother.h"

using namespace std;
using namespace Service;
using namespace Service::Imaging;
using namespace Service::Modeling;
using namespace Service::Smoothing;

void handle_part_image(short***, short, short, short, short);

int main(int argc, char *argv[])
{
	//Load tomography slices
	ApplicationFactory::Initializer(argv);

	//Get slice data
	short columns = ApplicationFactory::columns;
	short rows = ApplicationFactory::rows;
	int image_count = ApplicationFactory::image_count;
	short*** voxels = ApplicationFactory::getImages();

	//It's spacing between near pixels
	float dx = ApplicationFactory::x_pixelSpacing;
	float dy = ApplicationFactory::y_pixelSpacing;
	float dz = ApplicationFactory::sliceSpacing;

	thread thread1(handle_part_image, voxels, 0, image_count / 2, rows, columns);
	thread thread2(handle_part_image, voxels, image_count / 2, image_count, rows, columns);

	thread1.join();
	thread2.join();

	short iso_surface = 150;
	string fileName = "D:/Study/Kursach/Project/Models/Test33.stl";

	//Building model
	MarchingCube cube(voxels, image_count, rows, columns, dx, dy, dz, false);
	cube.march(iso_surface);

	//Getting built triangle model
	list<Triangle> triangles = cube.getTriangleList();

	//Clearing Ram from loaded slices
	ApplicationFactory::clear();

	//Writing model to binary stl file
	cube.recordToBinarySTL(fileName);

	/*string fileName1 = "D:/Study/Kursach/Project/Models/TestSmoothed33.stl"; 
	Smoother smoother(triangles);*/

	//Using Taubin smooth algorithm for model with Fujiwara operator
	/*smoother.TaubinSmooth(0.55f, -0.6f, 20);
	smoother.recordToBinarySTL(fileName1);*/

	cout << "Working have been finished!\n";
	getch();
	return 0;
}

//This function handle images by matrix filters 
void handle_part_image(short*** voxels, short start_index_slice, short image_count, short rows, short columns)
{
	if (image_count == 0)
	{
		cout << "Images was not found!";
		exit(1);
	}

	//string file_name = "D:/Study/Kursach/Project/Models/file.bin";
	for (int i = start_index_slice; i < image_count; ++i)
	{
		MorfologicalFilter filter(voxels[i], rows, columns);
		filter.GaussianFilter();

		cout << "Slice " << i + 1 << " of " << image_count << "\n";
		//filter.writeToFile(file_name);
		voxels[i] = filter.getHandledSlice();
	}
}
