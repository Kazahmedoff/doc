//STL including
#include <string>

//input/output console
#include <conio.h>

#include "ApplicationFactory.h"

#include "filter.h"
#include "MorfologicalFilter.h"

#include "MarchingCube.h"

#include "mesh.h"

using namespace std;
using namespace Service;
using namespace Service::Image;
using namespace Service::Model;
using namespace Service::Smoothing;

using namespace boost::filesystem;
using namespace boost::lambda;

int main(int argc, char *argv[])
{
	ApplicationFactory::Initializer(argv);

	short columns = ApplicationFactory::columns;
	short rows = ApplicationFactory::rows;
	int image_count = ApplicationFactory::image_count;
	short*** voxels = ApplicationFactory::getImages();

	//It's spacing between near pixels
	float dx = ApplicationFactory::x_pixelSpacing;
	float dy = ApplicationFactory::y_pixelSpacing;
	float dz = ApplicationFactory::sliceSpacing;

	/*for (size_t i = 0; i < rows; ++i)
	{
	const char* pointer = reinterpret_cast<const char*>(&slice[i][0]);
	DataPixels.write(pointer, sizeof(slice[i][0])*columns);
	}*/

	for (int i = 0; i < image_count; ++i)
	{
		Filter *filter = new Filter(voxels[i], rows, columns);
		filter->GaussianFilter();

		cout << "Slice " << i + 1 << " of " << image_count << "\n";

		voxels[i] = filter->GetHandledSlice();
		delete filter;
	}

	short iso_surface = 100;
	string fileName = "D:/Study/Kursach/Project/Models/experiment.stl";

	MarchingCube cube(voxels, image_count, rows, columns, dx, dy, dz);

	cube.march(iso_surface);

	ApplicationFactory::clear();

	list<Triangle> triangles = cube.getTriangleList();

	/*Mesh *object = new Mesh(triangles);
	object->TaubinSmooth(0.5f, -0.53f, 10);
	triangles = object->getSmoothedMesh();*/

	cube.SaveToSTL(fileName);

	getch();
	return 0;
}
