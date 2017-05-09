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

void MarchingCube::march(short iso_surface) 
{
	cout << "Building model..." << "\n";

	for (int k = 0; k < sz - 1; ++k) {
		for (int j = 0; j < sy - 1; ++j) {
			for (int i = 0; i < sx - 1; ++i) {
				Builder builder(dx, dy, dz, iso_surface);
				float x = i * dx;
				float y = j * dy;
				float z = k * dz;
				builder.setValues(voxels, x, y, z);
				this->triangles.splice(this->triangles.end(), builder.getTriangles(voxels, x, y, z));
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
		}

		else
		{
			++it;
		}
		i++;
	}
}

void MarchingCube::recordToBinarySTL(string fileName)
{
	ofstream model(fileName.c_str(), ios_base::binary | ios_base::trunc);

	vector<char> buffer(80, '0');
	unsigned int triangle_num = this->triangles.size();

	cout << "Writing data..." << "\n";

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
	cout << "Successful writing!";
}

void MarchingCube::recordToPLY(string fileName)
{
	ofstream model(fileName.c_str(), ios_base::trunc);

	char format[] = "ply";
	char extansion[] = "format ascii 1.0";

	char ch[] = "element vertex ";
	unsigned int n = 3 * triangles.size();;
	char numb[10] = { 0 };
	itoa(n, numb, 10);
	char vertices_number[24] = { 0 };
	strcat(vertices_number, ch);
	strcat(vertices_number, numb);

	char prop_x[] = "property float x";
	char prop_y[] = "property float y";
	char prop_z[] = "property float z";

	char ch_[] = "element face ";
	n = triangles.size();
	char numb_[10] = { 0 };
	itoa(n, numb_, 10);
	char face[21] = { 0 };
	strcat(face, ch_);
	strcat(face, numb_);;

	char vert_indices[] = "property list uint uint vertex_indices";
	char end_header[] = "end_header";

	model << format << "\n";
	model << extansion << "\n";
	model << vertices_number << "\n";
	model << prop_x << "\n";
	model << prop_y << "\n";
	model << prop_z << "\n";
	model << face << "\n";
	model << vert_indices << "\n";
	model << end_header << "\n";

	for (Triangle triangle : triangles)
	{
		for (short i = 0; i < 3; ++i)
		{
			model << triangle.v[i].x << " ";
			model << triangle.v[i].y << " ";
			model << triangle.v[i].z << "\n";
		}
	}

	unsigned int v = 3;
	unsigned int index = 0;
	for (Triangle triangle : triangles)
	{
		model << v << " ";
		model << index << " ";
		index++;

		model << index << " ";
		index++;

		model << index << "\n";
		index++;
	}
	model.close();
}

list <Triangle> MarchingCube::getTriangleList()
{
	return this->triangles;
}