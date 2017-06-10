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
	ApplicationFactory::Initializer(argv);

	short columns = ApplicationFactory::columns;
	short rows = ApplicationFactory::rows;
	int image_count = ApplicationFactory::image_count;
	short*** voxels = ApplicationFactory::getImages();

	//It's spacing between near pixels
	float dx = ApplicationFactory::x_pixelSpacing;
	float dy = ApplicationFactory::y_pixelSpacing;
	float dz = ApplicationFactory::sliceSpacing;
	//string file_name = "D:/Study/Kursach/Project/Models/ModelZverevaGaussian.stl";

	thread thread1(handle_part_image, voxels, 0, image_count / 2, rows, columns);
	thread thread2(handle_part_image, voxels, image_count / 2, image_count, rows, columns);

	thread1.join();
	thread2.join();

	short iso_surface = 150;
	string fileName = "D:/Study/Kursach/Project/Models/ModelZverevaGaussian.stl";

	MarchingCube cube(voxels, image_count, rows, columns, dx, dy, dz);
	cube.march(iso_surface);
	list<Triangle> triangles = cube.getTriangleList();

	ApplicationFactory::clear();
	cube.recordToBinarySTL(fileName);

	/*string fileName1 = "D:/Study/Kursach/Project/Models/SmoothedModelKhanikyanGaussian.stl"; 
 
	Smoother smoother(triangles);
	smoother.TaubinSmooth(0.55f, -0.6f, 17);
	smoother.recordToBinarySTL(fileName1);*/

	cout << "Working have been finished!\n";
	getch();
	return 0;
}

void handle_part_image(short*** voxels, short start_index_slice, short image_count, short rows, short columns)
{
	if (image_count == 0)
		exit(1);

	for (int i = start_index_slice; i < image_count; ++i)
	{
		Filter filter(voxels[i], rows, columns);
		filter.GaussianFilter();

		cout << "Slice " << i + 1 << " of " << image_count << "\n";
		//filter.WriteToFile(file_name);
		voxels[i] = filter.GetHandledSlice();
	}
}
