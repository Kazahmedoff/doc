#include "MarchingCube.h"
#include "Recorder.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
using namespace Service::Modeling;
using namespace Service::Saving;

MarchingCube::MarchingCube(short*** voxels, short sz, short sy, short sx, float dx, float dy, float dz) {
	this->voxels = voxels;
	this->sx = sx;
	this->sy = sy;
	this->sz = sz;
	this->dx = dx;
	this->dy = dy;
	this->dz = dz;
}

MarchingCube::MarchingCube(string filename) {
	ifstream mhaReader;
	mhaReader.open(filename);
	if (mhaReader.is_open()) {
		short val;
		mhaReader >> this->sx;
		mhaReader >> this->sy;
		mhaReader >> this->sz;
		mhaReader >> this->dx;
		mhaReader >> this->dy;
		mhaReader >> this->dz;

		voxels = new short **[sz];
		for (int k = 0; k < sz; k++) {
			voxels[k] = new short *[sy];
			for (int j = 0; j < sy; j++) {
				voxels[k][j] = new short[sz];
			}
		}

		int max = 0;

		for (int k = 0; k < sz; k++) {
			for (int j = 0; j < sy; j++) {
				for (int i = 0; i < sx; i++) {
					mhaReader >> val;
					voxels[k][j][i] = val;
					if (val > max) {
						max = val;
					}
				}
			}
		}
	}
}

void MarchingCube::march(short iso_surface) 
{
	cout << "Building model..." << "\n";
	Builder builder(dx, dy, dz, iso_surface);

	for (int k = 0; k < sz - 1; ++k) {
		for (int j = 0; j < sy - 1; ++j) {
			for (int i = 0; i < sx - 1; ++i) {

				if (builder.setValues(voxels, i, j, k))
					this->triangles.splice(this->triangles.end(), builder.getTriangles());

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

void MarchingCube::recordToBinarySTL(string fileName)
{
	Recodrer::recordModelToBinarySTL(triangles, fileName);
}

void MarchingCube::recordToPLY(string fileName)
{
	Recodrer::recordModelToPLY(triangles, fileName);
}

void MarchingCube::recordToSTL(string fileName)
{
	Recodrer::recordModelToSTL(triangles, fileName);
}

list <Triangle> MarchingCube::getTriangleList()
{
	return this->triangles;
}