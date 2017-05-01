#include "MarchingCube.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
using namespace Service::Model;

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

void MarchingCube::march(short iso_surface) {
	for (int k = 0; k < sz - 1; ++k) {
		for (int j = 0; j < sy - 1; ++j) {
			for (int i = 0; i < sx - 1; ++i) {
				EdgeTable edgeTable(dx, dy, dz);
				float x = i * dx;
				float y = j * dy;
				float z = k * dz;
				edgeTable.setValues(voxels, x, y, z, iso_surface);
				this->triangles.splice(this->triangles.end(), edgeTable.getTriangles(voxels, x, y, z, iso_surface));
			}
		}
	}
}

void MarchingCube::SaveToSTL(string fileName)
{
	ofstream model(fileName.c_str(), ios_base::binary | ios_base::trunc);

	vector<char> buffer(80, '0');
	unsigned int triangle_num = this->triangles.size();

	model.write(reinterpret_cast<const char *>(&buffer[0]), 80);
	model.write(reinterpret_cast<const char *>(&triangle_num), sizeof(triangle_num));
	unsigned short attribute_byte_count = 0;

	for (Triangle triangle : this->triangles)
	{
		float buffer_x = triangle.normal.Normalize().Nx;
		float buffer_y = triangle.normal.Normalize().Ny;
		float buffer_z = triangle.normal.Normalize().Nz;

		model.write(reinterpret_cast<const char *>(&buffer_x), sizeof(buffer_x));
		model.write(reinterpret_cast<const char *>(&buffer_y), sizeof(buffer_y));
		model.write(reinterpret_cast<const char *>(&buffer_z), sizeof(buffer_z));

		for (short i = 0; i < 3; ++i)
		{
			buffer_x = triangle.v[i].x;
			buffer_y = triangle.v[i].y;
			buffer_z = triangle.v[i].z;

			model.write(reinterpret_cast<const char *>(&buffer_x), sizeof(buffer_x));
			model.write(reinterpret_cast<const char *>(&buffer_y), sizeof(buffer_y));
			model.write(reinterpret_cast<const char *>(&buffer_z), sizeof(buffer_z));
		}

		model.write(reinterpret_cast<const char *>(&attribute_byte_count), sizeof(attribute_byte_count));
	}

	model.close();
}

list <Triangle> MarchingCube::getTriangleList()
{
	return this->triangles;
}