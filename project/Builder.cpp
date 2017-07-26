#include "Builder.h"

using namespace Service::Modeling;

Builder::Builder() { }

Builder::Builder(float dx, float dy, float dz, short iso_surface, bool standartMC)
{
	this->dx = dx;
	this->dy = dy;
	this->dz = dz;
	this->iso_surface = iso_surface;
	this->standartMC = standartMC;
}

void Builder::getVertices(short edge, short *arr) {
	switch (edge) {
	case 0:
		arr[0] = 0;
		arr[1] = 1;
		break;
	case 1:
		arr[0] = 1;
		arr[1] = 2;
		break;
	case 2:
		arr[0] = 2;
		arr[1] = 3;
		break;
	case 3:
		arr[0] = 3;
		arr[1] = 0;
		break;
	case 4:
		arr[0] = 4;
		arr[1] = 5;
		break;
	case 5:
		arr[0] = 5;
		arr[1] = 6;
		break;
	case 6:
		arr[0] = 6;
		arr[1] = 7;
		break;
	case 7:
		arr[0] = 7;
		arr[1] = 4;
		break;
	case 8:
		arr[0] = 0;
		arr[1] = 4;
		break;
	case 9:
		arr[0] = 1;
		arr[1] = 5;
		break;
	case 10:
		arr[0] = 2;
		arr[1] = 6;
		break;
	case 11:
		arr[0] = 3;
		arr[1] = 7;
		break;
	}
}

bool Builder::setValues(short*** voxels, short i, short j, short k) {

	cell.vertex[0].x = i * dx;
	cell.vertex[0].y = j * dy;
	cell.vertex[0].z = k * dz;
	cell.value[0] = voxels[k][j][i] - iso_surface;

	cell.vertex[1].x = (i + 3) * dx;
	cell.vertex[1].y = j * dy;
	cell.vertex[1].z = k * dz;
	cell.value[1] = voxels[k][j][i + 3] - iso_surface;

	cell.vertex[2].x = (i + 3) * dx;
	cell.vertex[2].y = j * dy;
	cell.vertex[2].z = (k + 1) * dz;
	cell.value[2] = voxels[k + 1][j][i + 3] - iso_surface;

	cell.vertex[3].x = i * dx;
	cell.vertex[3].y = j * dy;
	cell.vertex[3].z = (k + 1) * dz;
	cell.value[3] = voxels[k + 1][j][i] - iso_surface;

	cell.vertex[4].x = i * dx;
	cell.vertex[4].y = (j + 3) * dy;
	cell.vertex[4].z = k * dz;
	cell.value[4] = voxels[k][j + 3][i] - iso_surface;

	cell.vertex[5].x = (i + 3) * dx;
	cell.vertex[5].y = (j + 3) * dy;
	cell.vertex[5].z = k * dz;
	cell.value[5] = voxels[k][j + 3][i + 3] - iso_surface;

	cell.vertex[6].x = (i + 3) * dx;
	cell.vertex[6].y = (j + 3) * dy;
	cell.vertex[6].z = (k + 1) * dz;
	cell.value[6] = voxels[k + 1][j + 3][i + 3] - iso_surface;

	cell.vertex[7].x = i * dx;
	cell.vertex[7].y = (j + 3) * dy;
	cell.vertex[7].z = (k + 1) * dz;
	cell.value[7] = voxels[k + 1][j + 3][i] - iso_surface;

	short count_ = 0;
	for (int i = 0; i < 8; ++i)
	{
		if (fabs(cell.value[i]) < FLT_EPSILON)
			cell.value[i] = FLT_EPSILON;

		cell.nodeParity[i] = cell.value[i] > 0;

		if (cell.nodeParity[i])
			count_++;
	}

	if (count_ > 0 && count_ < 8)
		return true;

	else
		return false;
}

Vertex Builder::getIntersection(short edge) 
{
	if (edge == 12)
		return cell.additional_vertex;

	short vertices[2];
	getVertices(edge, vertices);

	Vertex v1 = cell.vertex[vertices[0]];
	Vertex v2 = cell.vertex[vertices[1]];

	float value1 = cell.value[vertices[0]];
	float value2 = cell.value[vertices[1]];

	if (abs(iso_surface - value1) == 0)
		return v1;
	if (abs(iso_surface - value2) == 0)
		return v2;
	if (abs(value1 - value2) == 0)
		return v1;

	float scale = (1.0 * (iso_surface - value1)) / (value2 - value1);

	Vertex v;
	v = v1 + (v2 - v1) * scale;

	return v;
}

short Builder::getNodeCaseNumber()
{
	short powerOf2 = 1;
	short caseID = 0;
	for (int i = 0; i < 8; i++) 
	{
		if (cell.nodeParity[i])
			caseID += powerOf2;

		powerOf2 *= 2;
	}
	return caseID;
}

void Builder::addTriangles(list<Triangle> &triangles, const __int8 edges[], short triangles_count)
{
	short index = 0;
	for (short i = 0; i < triangles_count; ++i)
	{
		Vertex v[3];

		v[0] = getIntersection(edges[index]);
		v[1] = getIntersection(edges[index + 1]);
		v[2] = getIntersection(edges[index + 2]);

		Triangle triangle(v);
		triangle.normal = triangle.normal.Normalize();

		triangles.push_back(triangle);
		index += 3;
	}
}

list <Triangle> Builder::getTriangles() 
{
	list <Triangle> triangles(0);
	short index = 0;
	short nodeCase = getNodeCaseNumber();

	if (standartMC)
	{
		while (index < 16 && classicCases[nodeCase][index] != -1)
		{
			Vertex v[3];

			v[0] = getIntersection(classicCases[nodeCase][index]);
			v[1] = getIntersection(classicCases[nodeCase][index + 1]);
			v[2] = getIntersection(classicCases[nodeCase][index + 2]);

			Triangle triangle(v);
			triangle.normal = triangle.normal.Normalize();

			triangles.push_back(triangle);
			index += 3;
		}
		return triangles;
	}

	//MC33 begining
	short _case = cases[nodeCase][0];
	short _config = cases[nodeCase][1];
	short _subconfig = 0;
	tunnelOrientation = 0;

	cell.additional_vertex = Vertex(0, 0, 0);
	short count_ = 0;

	for (short i = 0; i < 8; ++i)
	{
		//if (cell.nodeParity[i])
		//{
			cell.additional_vertex += cell.vertex[i];
			count_++;
		//}
	}
	cell.additional_vertex = cell.additional_vertex / static_cast<float>(count_);

	switch (_case) {

	case 0:
		break;

	case 1:
		addTriangles(triangles, tiling1[_config], 1);
		break;

	case 2:
		addTriangles(triangles, tiling2[_config], 2);
		break;

	case 3:
		if (testFace(test3[_config]))
			addTriangles(triangles, tiling3_2[_config], 4); // 3.2
		else
			addTriangles(triangles, tiling3_1[_config], 2); // 3.1
		break;

	case 4:
		if (modifiedTestInterior(test4[_config], _case, _config))
			addTriangles(triangles, tiling4_1[_config], 2); // 4.1.1
		else
			addTriangles(triangles, tiling4_2[_config], 6); // 4.1.2
		break;

	case 5:
		addTriangles(triangles, tiling5[_config], 3);
		break;

	case 6:
		if (testFace(test6[_config][0]))
			addTriangles(triangles, tiling6_2[_config], 5); // 6.2
		else {
			if (modifiedTestInterior(test6[_config][1], _case, _config))
				addTriangles(triangles, tiling6_1_1[_config], 3); // 6.1.1
			else {
				 
				addTriangles(triangles, tiling6_1_2[_config], 9 ); // 6.1.2
			}
		}
		break;

	case 7:
		if (testFace(test7[_config][0]))
			_subconfig += 1;
		if (testFace(test7[_config][1]))
			_subconfig += 2;
		if (testFace(test7[_config][2]))
			_subconfig += 4;
		switch (_subconfig) {
		case 0:
			addTriangles(triangles, tiling7_1[_config], 3);
			break;
		case 1:
			addTriangles(triangles, tiling7_2[_config][0], 5);
			break;
		case 2:
			addTriangles(triangles, tiling7_2[_config][1], 5);
			break;
		case 3:
			 
			addTriangles(triangles, tiling7_3[_config][0], 9 );
			break;
		case 4:
			addTriangles(triangles, tiling7_2[_config][2], 5);
			break;
		case 5:
			 
			addTriangles(triangles, tiling7_3[_config][1], 9 );
			break;
		case 6:
			 
			addTriangles(triangles, tiling7_3[_config][2], 9 );
			break;
		case 7:
			if (modifiedTestInterior(test7[_config][3], _case, _config))
				addTriangles(triangles, tiling7_4_1[_config], 5);
			else
				addTriangles(triangles, tiling7_4_2[_config], 9);
			break;
		}
		break;

	case 8:
		addTriangles(triangles, tiling8[_config], 2);
		break;

	case 9:
		addTriangles(triangles, tiling9[_config], 4);
		break;

	case 10:
		if (testFace(test10[_config][0])) {
			if (testFace(test10[_config][1])) {
				if (modifiedTestInterior(-test10[_config][2], _case, _config))
					addTriangles(triangles, tiling10_1_1_[_config], 4); // 10.1.1
				else
					addTriangles(triangles, tiling10_1_2[5 - _config], 8); // 10.1.2

			}
			else {
				 
				addTriangles(triangles, tiling10_2[_config], 8 ); // 10.2
			}
		}
		else {
			if (testFace(test10[_config][1])) {
				 
				addTriangles(triangles, tiling10_2_[_config], 8 ); // 10.2
			}
			else {
				if (modifiedTestInterior(test10[_config][2], _case, _config))
					addTriangles(triangles, tiling10_1_1[_config], 4); // 10.1.1
				else
					addTriangles(triangles, tiling10_1_2[_config], 8); // 10.1.2
			}
		}
		break;

	case 11:
		addTriangles(triangles, tiling11[_config], 4);
		break;

	case 12:
		if (testFace(test12[_config][0])) {
			if (testFace(test12[_config][1])) {
				if (modifiedTestInterior(-test12[_config][2], _case, _config))
					addTriangles(triangles, tiling12_1_1_[_config], 4); // 12.1.1
				else
					addTriangles(triangles, tiling12_1_2[23 - _config], 8); // 12.1.2
			}
			else {
				 
				addTriangles(triangles, tiling12_2[_config], 8 ); // 12.2
			}
		}
		else {
			if (testFace(test12[_config][1])) {
				 
				addTriangles(triangles, tiling12_2_[_config], 8 ); // 12.2
			}
			else {
				if (modifiedTestInterior(test12[_config][2], _case, _config))
					addTriangles(triangles, tiling12_1_1[_config], 4); // 12.1.1
				else
					addTriangles(triangles, tiling12_1_2[_config], 8); // 12.1.2
			}
		}
		break;

	case 13:
		if (testFace(test13[_config][0]))
			_subconfig += 1;
		if (testFace(test13[_config][1]))
			_subconfig += 2;
		if (testFace(test13[_config][2]))
			_subconfig += 4;
		if (testFace(test13[_config][3]))
			_subconfig += 8;
		if (testFace(test13[_config][4]))
			_subconfig += 16;
		if (testFace(test13[_config][5]))
			_subconfig += 32;

		switch (subconfig13[_subconfig]) 
		{
		case 0:/* 13.1 */
			addTriangles(triangles, tiling13_1[_config], 4);
			break;

		case 1:/* 13.2 */
			addTriangles(triangles, tiling13_2[_config][0], 6);
			break;
		case 2:/* 13.2 */
			addTriangles(triangles, tiling13_2[_config][1], 6);
			break;
		case 3:/* 13.2 */
			addTriangles(triangles, tiling13_2[_config][2], 6);
			break;
		case 4:/* 13.2 */
			addTriangles(triangles, tiling13_2[_config][3], 6);
			break;
		case 5:/* 13.2 */
			addTriangles(triangles, tiling13_2[_config][4], 6);
			break;
		case 6:/* 13.2 */
			addTriangles(triangles, tiling13_2[_config][5], 6);
			break;

		case 7:/* 13.3 */
			 
			addTriangles(triangles, tiling13_3[_config][0], 10 );
			break;
		case 8:/* 13.3 */
			 
			addTriangles(triangles, tiling13_3[_config][1], 10 );
			break;
		case 9:/* 13.3 */
			 
			addTriangles(triangles, tiling13_3[_config][2], 10 );
			break;
		case 10:/* 13.3 */
			 
			addTriangles(triangles, tiling13_3[_config][3], 10 );
			break;
		case 11:/* 13.3 */
			 
			addTriangles(triangles, tiling13_3[_config][4], 10 );
			break;
		case 12:/* 13.3 */
			 
			addTriangles(triangles, tiling13_3[_config][5], 10 );
			break;
		case 13:/* 13.3 */
			 
			addTriangles(triangles, tiling13_3[_config][6], 10 );
			break;
		case 14:/* 13.3 */
			 
			addTriangles(triangles, tiling13_3[_config][7], 10 );
			break;
		case 15:/* 13.3 */
			 
			addTriangles(triangles, tiling13_3[_config][8], 10 );
			break;
		case 16:/* 13.3 */
			 
			addTriangles(triangles, tiling13_3[_config][9], 10 );
			break;
		case 17:/* 13.3 */
			 
			addTriangles(triangles, tiling13_3[_config][10], 10 );
			break;
		case 18:/* 13.3 */
			 
			addTriangles(triangles, tiling13_3[_config][11], 10 );
			break;

		case 19:/* 13.4 */
			 
			addTriangles(triangles, tiling13_4[_config][0], 12 );
			break;
		case 20:/* 13.4 */
			 
			addTriangles(triangles, tiling13_4[_config][1], 12 );
			break;
		case 21:/* 13.4 */
			 
			addTriangles(triangles, tiling13_4[_config][2], 12 );
			break;
		case 22:/* 13.4 */
			 
			addTriangles(triangles, tiling13_4[_config][3], 12 );
			break;

		case 23:/* 13.5 */
			_subconfig = 0;
			if (_config == 0) {
				if (interiorTestCase13())
					addTriangles(triangles, tiling13_5_1[0][0], 6);
				else {
					if (tunnelOrientation == 1)
						addTriangles(triangles, tiling13_5_2[0][0], 10);
					else
						addTriangles(triangles, tiling13_5_2[1][2], 10);
				}
			}
			else {
				if (interiorTestCase13())
					addTriangles(triangles, tiling13_5_1[1][0], 6);
				else {
					if (tunnelOrientation == 1)
						addTriangles(triangles, tiling13_5_2[1][0], 10);
					else
						addTriangles(triangles, tiling13_5_2[0][2], 10);
				}
			}
			break;

		case 24:/* 13.5 */
			_subconfig = 1;
			if (_config == 0) {
				if (interiorTestCase13())
					addTriangles(triangles, tiling13_5_1[0][1], 6);
				else {
					if (tunnelOrientation == 1)
						addTriangles(triangles, tiling13_5_2[0][1], 10);
					else
						addTriangles(triangles, tiling13_5_2[1][0], 10);
				}
			}
			else
			{
				if (interiorTestCase13())
					addTriangles(triangles, tiling13_5_1[1][1], 6);
				else {
					if (tunnelOrientation == 1)
						addTriangles(triangles, tiling13_5_2[1][1], 10);
					else
						addTriangles(triangles, tiling13_5_2[0][3], 10);
				}
			}

			break;

		case 25:/* 13.5 */
			_subconfig = 2;
			if (_config == 0)
			{
				if (interiorTestCase13())
					addTriangles(triangles, tiling13_5_1[0][2], 6);
				else {
					if (tunnelOrientation == 1)
						addTriangles(triangles, tiling13_5_2[0][2], 10);
					else
						addTriangles(triangles, tiling13_5_2[1][3], 10);
				}
			}
			else
			{
				if (interiorTestCase13())
					addTriangles(triangles, tiling13_5_1[1][2], 6);
				else {
					if (tunnelOrientation == 1)
						addTriangles(triangles, tiling13_5_2[1][2], 10);
					else
						addTriangles(triangles, tiling13_5_2[0][0], 10);
				}

			}
			break;

		case 26: /* 13.5 */
			_subconfig = 3;
			if (_config == 0)
			{
				if (interiorTestCase13())
					addTriangles(triangles, tiling13_5_1[0][3], 6);
				else {
					if (tunnelOrientation == 1)
						addTriangles(triangles, tiling13_5_2[0][3], 10);
					else
						addTriangles(triangles, tiling13_5_2[1][1], 10);
				}
			}
			else
			{
				if (interiorTestCase13())
					addTriangles(triangles, tiling13_5_1[1][3], 6);
				else {
					if (tunnelOrientation == 1)
						addTriangles(triangles, tiling13_5_2[1][3], 10);
					else
						addTriangles(triangles, tiling13_5_2[0][2], 10);
				}
			}
			/* 13.4  common node is negative*/
			// v12 = add_c_vertex() ;
			// addTriangles( triangles, tiling13_4[_config][3], 12  ) ;
			break;

		case 27:/* 13.3 */
			 
			addTriangles(triangles, tiling13_3_[_config][0], 10 );
			break;
		case 28:/* 13.3 */
			 
			addTriangles(triangles, tiling13_3_[_config][1], 10 );
			break;
		case 29:/* 13.3 */
			 
			addTriangles(triangles, tiling13_3_[_config][2], 10 );
			break;
		case 30:/* 13.3 */
			 
			addTriangles(triangles, tiling13_3_[_config][3], 10 );
			break;
		case 31:/* 13.3 */
			 
			addTriangles(triangles, tiling13_3_[_config][4], 10 );
			break;
		case 32:/* 13.3 */
			 
			addTriangles(triangles, tiling13_3_[_config][5], 10 );
			break;
		case 33:/* 13.3 */
			 
			addTriangles(triangles, tiling13_3_[_config][6], 10 );
			break;
		case 34:/* 13.3 */
			 
			addTriangles(triangles, tiling13_3_[_config][7], 10 );
			break;
		case 35:/* 13.3 */
			 
			addTriangles(triangles, tiling13_3_[_config][8], 10 );
			break;
		case 36:/* 13.3 */
			 
			addTriangles(triangles, tiling13_3_[_config][9], 10 );
			break;
		case 37:/* 13.3 */
			 
			addTriangles(triangles, tiling13_3_[_config][10], 10 );
			break;
		case 38:/* 13.3 */
			 
			addTriangles(triangles, tiling13_3_[_config][11], 10 );
			break;

		case 39:/* 13.2 */
			addTriangles(triangles, tiling13_2_[_config][0], 6);
			break;
		case 40:/* 13.2 */
			addTriangles(triangles, tiling13_2_[_config][1], 6);
			break;
		case 41:/* 13.2 */
			addTriangles(triangles, tiling13_2_[_config][2], 6);
			break;
		case 42:/* 13.2 */
			addTriangles(triangles, tiling13_2_[_config][3], 6);
			break;
		case 43:/* 13.2 */
			addTriangles(triangles, tiling13_2_[_config][4], 6);
			break;
		case 44:/* 13.2 */
			addTriangles(triangles, tiling13_2_[_config][5], 6);
			break;

		case 45:/* 13.1 */
			addTriangles(triangles, tiling13_1_[_config], 4);
			break;

		default:
			cout << "Marching Cubes: Impossible case 13\n";
		}
		break;

	case 14:
		addTriangles(triangles, tiling14[_config], 4);
		break;
	};

	return triangles;
}

bool Builder::testFace(__int8 face)
{
	float A, B, C, D;

	switch (face) {
	case -1:
	case 1:
		A = cell.value[0];
		B = cell.value[4];
		C = cell.value[5];
		D = cell.value[1];
		break;
	case -2:
	case 2:
		A = cell.value[1];
		B = cell.value[5];
		C = cell.value[6];
		D = cell.value[2];
		break;
	case -3:
	case 3:
		A = cell.value[2];
		B = cell.value[6];
		C = cell.value[7];
		D = cell.value[3];
		break;
	case -4:
	case 4:
		A = cell.value[3];
		B = cell.value[7];
		C = cell.value[4];
		D = cell.value[0];
		break;
	case -5:
	case 5:
		A = cell.value[0];
		B = cell.value[3];
		C = cell.value[2];
		D = cell.value[1];
		break;
	case -6:
	case 6:
		A = cell.value[4];
		B = cell.value[7];
		C = cell.value[6];
		D = cell.value[5];
		break;

	default:
		cout << "Invalid face code " << face << "\n";
		A = B = C = D = 0;
		break;
	};

	if (fabs(A * C - B * D) < 0)
		return face >= 0;

	return face * A * (A * C - B * D) >= 0; // face and A invert signs
}

bool Builder::modifiedTestInterior(__int8 s, short _case, short _config)
{
	__int8 edge = -1;
	short amb_face;

	short inter_amb = 0;

	switch (_case) {
	case 4:
		amb_face = 1;
		edge = interiorAmbiguity(amb_face, s);
		inter_amb += interiorAmbiguityVerification(edge);

		amb_face = 2;
		edge = interiorAmbiguity(amb_face, s);
		inter_amb += interiorAmbiguityVerification(edge);

		amb_face = 5;
		edge = interiorAmbiguity(amb_face, s);
		inter_amb += interiorAmbiguityVerification(edge);
		break;

	case 6:
		amb_face = abs(test6[_config][0]);
		edge = interiorAmbiguity(amb_face, s);
		inter_amb = interiorAmbiguityVerification(edge);
		break;

	case 7:
		s = -s;

		amb_face = 1;
		edge = interiorAmbiguity(amb_face, s);
		inter_amb += interiorAmbiguityVerification(edge);

		amb_face = 2;
		edge = interiorAmbiguity(amb_face, s);
		inter_amb += interiorAmbiguityVerification(edge);

		amb_face = 5;
		edge = interiorAmbiguity(amb_face, s);
		inter_amb += interiorAmbiguityVerification(edge);

	case 10:
		amb_face = abs(test10[_config][0]);
		edge = interiorAmbiguity(amb_face, s);
		inter_amb = interiorAmbiguityVerification(edge);
		break;

	case 12:
		amb_face = abs(test12[_config][0]);
		edge = interiorAmbiguity(amb_face, s);
		inter_amb += interiorAmbiguityVerification(edge);


		amb_face = abs(test12[_config][1]);
		edge = interiorAmbiguity(amb_face, s);
		inter_amb += interiorAmbiguityVerification(edge);
		break;
	}

	if (inter_amb == 0)
		return false;
	else
		return true;
}

__int8 Builder::interiorAmbiguity(short amb_face, __int8 s)
{
	__int8 edge = -1;

	switch (amb_face) {
	case 1:
	case 3:
		if (((cell.value[1] * s) > 0) && ((cell.value[7] * s) > 0))
			edge = 4;
		if (((cell.value[0] * s) > 0) && ((cell.value[6] * s) > 0))
			edge = 5;
		if (((cell.value[3] * s) > 0) && ((cell.value[5] * s) > 0))
			edge = 6;
		if (((cell.value[2] * s) > 0) && ((cell.value[4] * s) > 0))
			edge = 7;
		break;

	case 2:
	case 4:
		if (((cell.value[1] * s) > 0) && ((cell.value[7] * s) > 0))
			edge = 0;
		if (((cell.value[2] * s) > 0) && ((cell.value[4] * s) > 0))
			edge = 1;
		if (((cell.value[3] * s) > 0) && ((cell.value[5] * s) > 0))
			edge = 2;
		if (((cell.value[0] * s) > 0) && ((cell.value[6] * s) > 0))
			edge = 3;
		break;

	case 5:
	case 6:
	case 0:
		if (((cell.value[0] * s) > 0) && ((cell.value[6] * s) > 0))
			edge = 8;
		if (((cell.value[1] * s) > 0) && ((cell.value[7] * s) > 0))
			edge = 9;
		if (((cell.value[2] * s) > 0) && ((cell.value[4] * s) > 0))
			edge = 10;
		if (((cell.value[3] * s) > 0) && ((cell.value[5] * s) > 0))
			edge = 11;
		break;
	}

	return edge;
}

__int8 Builder::interiorAmbiguityVerification(__int8 edge)
{
	float t;
	float At = 0, Bt = 0, Ct = 0, Dt = 0;
	float a = 0, b = 0;
	float verify;

	switch (edge) 
	{

	case 0:
		a = (cell.value[0] - cell.value[1]) * (cell.value[7] - cell.value[6])
			- (cell.value[4] - cell.value[5]) * (cell.value[3] - cell.value[2]);
		b = cell.value[6] * (cell.value[0] - cell.value[1]) + cell.value[1] * (cell.value[7] - cell.value[6])
			- cell.value[2] * (cell.value[4] - cell.value[5])
			- cell.value[5] * (cell.value[3] - cell.value[2]);

		if (a > 0)
			return 1;

		t = -b / static_cast<float>(2 * a);
		if (t < 0 || t > 1)
			return 1;

		At = cell.value[1] + (cell.value[0] - cell.value[1]) * t;
		Bt = cell.value[5] + (cell.value[4] - cell.value[5]) * t;
		Ct = cell.value[6] + (cell.value[7] - cell.value[6]) * t;
		Dt = cell.value[2] + (cell.value[3] - cell.value[2]) * t;

		verify = (At * Ct) - (Bt * Dt);

		if (verify > 0)
			return 0;
		if (verify < 0)
			return 1;

		break;

	case 1:
		a = (cell.value[3] - cell.value[2]) * (cell.value[4] - cell.value[5])
			- (cell.value[0] - cell.value[1]) * (cell.value[7] - cell.value[6]);
		b = cell.value[5] * (cell.value[3] - cell.value[2]) + cell.value[2] * (cell.value[4] - cell.value[5])
			- cell.value[6] * (cell.value[0] - cell.value[1])
			- cell.value[1] * (cell.value[7] - cell.value[6]);

		if (a > 0)
			return 1;

		t = -b / static_cast<float>(2 * a);
		if (t < 0 || t > 1)
			return 1;

		At = cell.value[2] + (cell.value[3] - cell.value[2]) * t;
		Bt = cell.value[1] + (cell.value[0] - cell.value[1]) * t;
		Ct = cell.value[5] + (cell.value[4] - cell.value[5]) * t;
		Dt = cell.value[6] + (cell.value[7] - cell.value[6]) * t;

		verify = (At * Ct) - (Bt * Dt);

		if (verify > 0)
			return 0;
		if (verify < 0)
			return 1;
		break;

	case 2:
		a = (cell.value[2] - cell.value[3]) * (cell.value[5] - cell.value[4])
			- (cell.value[6] - cell.value[7]) * (cell.value[1] - cell.value[0]);
		b = cell.value[4] * (cell.value[2] - cell.value[3]) + cell.value[3] * (cell.value[5] - cell.value[4])
			- cell.value[0] * (cell.value[6] - cell.value[7])
			- cell.value[7] * (cell.value[1] - cell.value[0]);
		if (a > 0)
			return 1;

		t = -b / static_cast<float>(2 * a);
		if (t < 0 || t > 1)
			return 1;

		At = cell.value[3] + (cell.value[2] - cell.value[3]) * t;
		Bt = cell.value[7] + (cell.value[6] - cell.value[7]) * t;
		Ct = cell.value[4] + (cell.value[5] - cell.value[4]) * t;
		Dt = cell.value[0] + (cell.value[1] - cell.value[0]) * t;

		verify = (At * Ct) - (Bt * Dt);

		if (verify > 0)
			return 0;
		if (verify < 0)
			return 1;
		break;

	case 3:
		a = (cell.value[1] - cell.value[0]) * (cell.value[6] - cell.value[7])
			- (cell.value[2] - cell.value[3]) * (cell.value[5] - cell.value[4]);
		b = cell.value[7] * (cell.value[1] - cell.value[0]) + cell.value[0] * (cell.value[6] - cell.value[7])
			- cell.value[4] * (cell.value[2] - cell.value[3])
			- cell.value[3] * (cell.value[5] - cell.value[4]);
		if (a > 0)
			return 1;

		t = -b / static_cast<float>(2 * a);
		if (t < 0 || t > 1)
			return 1;

		At = cell.value[0] + (cell.value[1] - cell.value[0]) * t;
		Bt = cell.value[3] + (cell.value[2] - cell.value[3]) * t;
		Ct = cell.value[7] + (cell.value[6] - cell.value[7]) * t;
		Dt = cell.value[4] + (cell.value[5] - cell.value[4]) * t;

		verify = (At * Ct) - (Bt * Dt);

		if (verify > 0)
			return 0;
		if (verify < 0)
			return 1;
		break;

	case 4:

		a = (cell.value[2] - cell.value[1]) * (cell.value[7] - cell.value[4])
			- (cell.value[3] - cell.value[0]) * (cell.value[6] - cell.value[5]);
		b = cell.value[4] * (cell.value[2] - cell.value[1]) + cell.value[1] * (cell.value[7] - cell.value[4])
			- cell.value[5] * (cell.value[3] - cell.value[0])
			- cell.value[0] * (cell.value[6] - cell.value[5]);

		if (a > 0)
			return 1;

		t = -b / static_cast<float>(2 * a);
		if (t < 0 || t > 1)
			return 1;

		At = cell.value[1] + (cell.value[2] - cell.value[1]) * t;
		Bt = cell.value[0] + (cell.value[3] - cell.value[0]) * t;
		Ct = cell.value[4] + (cell.value[7] - cell.value[4]) * t;
		Dt = cell.value[5] + (cell.value[6] - cell.value[5]) * t;

		verify = (At * Ct) - (Bt * Dt);

		if (verify > 0)
			return 0;
		if (verify < 0)
			return 1;
		break;

	case 5:

		a = (cell.value[3] - cell.value[0]) * (cell.value[6] - cell.value[5])
			- (cell.value[2] - cell.value[1]) * (cell.value[7] - cell.value[4]);
		b = cell.value[5] * (cell.value[3] - cell.value[0]) + cell.value[0] * (cell.value[6] - cell.value[5])
			- cell.value[4] * (cell.value[2] - cell.value[1])
			- cell.value[1] * (cell.value[7] - cell.value[4]);
		if (a > 0)
			return 1;

		t = -b / static_cast<float>(2 * a);
		if (t < 0 || t > 1)
			return 1;

		At = cell.value[0] + (cell.value[3] - cell.value[0]) * t;
		Bt = cell.value[1] + (cell.value[2] - cell.value[1]) * t;
		Ct = cell.value[5] + (cell.value[6] - cell.value[5]) * t;
		Dt = cell.value[4] + (cell.value[7] - cell.value[4]) * t;

		verify = (At * Ct) - (Bt * Dt);

		if (verify > 0)
			return 0;
		if (verify < 0)
			return 1;
		break;

	case 6:
		a = (cell.value[0] - cell.value[3]) * (cell.value[5] - cell.value[6])
			- (cell.value[4] - cell.value[7]) * (cell.value[1] - cell.value[2]);
		b = cell.value[6] * (cell.value[0] - cell.value[3]) + cell.value[3] * (cell.value[5] - cell.value[6])
			- cell.value[2] * (cell.value[4] - cell.value[7])
			- cell.value[7] * (cell.value[1] - cell.value[2]);
		if (a > 0)
			return 1;

		t = -b / static_cast<float>(2 * a);
		if (t < 0 || t > 1)
			return 1;

		At = cell.value[3] + (cell.value[0] - cell.value[3]) * t;
		Bt = cell.value[7] + (cell.value[4] - cell.value[7]) * t;
		Ct = cell.value[6] + (cell.value[5] - cell.value[6]) * t;
		Dt = cell.value[2] + (cell.value[1] - cell.value[2]) * t;

		verify = (At * Ct) - (Bt * Dt);

		if (verify > 0)
			return 0;
		if (verify < 0)
			return 1;
		break;

	case 7:
		a = (cell.value[1] - cell.value[2]) * (cell.value[4] - cell.value[7])
			- (cell.value[0] - cell.value[3]) * (cell.value[5] - cell.value[6]);
		b = cell.value[7] * (cell.value[1] - cell.value[2]) + cell.value[2] * (cell.value[4] - cell.value[7])
			- cell.value[6] * (cell.value[0] - cell.value[3])
			- cell.value[3] * (cell.value[5] - cell.value[6]);
		if (a > 0)
			return 1;

		t = -b / static_cast<float>(2 * a);
		if (t < 0 || t > 1)
			return 1;

		At = cell.value[2] + (cell.value[1] - cell.value[2]) * t;
		Bt = cell.value[3] + (cell.value[0] - cell.value[3]) * t;
		Ct = cell.value[7] + (cell.value[4] - cell.value[7]) * t;
		Dt = cell.value[6] + (cell.value[5] - cell.value[6]) * t;

		verify = (At * Ct) - (Bt * Dt);

		if (verify > 0)
			return 0;
		if (verify < 0)
			return 1;
		break;

	case 8:
		a = (cell.value[4] - cell.value[0]) * (cell.value[6] - cell.value[2])
			- (cell.value[7] - cell.value[3]) * (cell.value[5] - cell.value[1]);
		b = cell.value[2] * (cell.value[4] - cell.value[0]) + cell.value[0] * (cell.value[6] - cell.value[2])
			- cell.value[1] * (cell.value[7] - cell.value[3])
			- cell.value[3] * (cell.value[5] - cell.value[1]);
		if (a > 0)
			return 1;

		t = -b / static_cast<float>(2 * a);
		if (t < 0 || t > 1)
			return 1;

		At = cell.value[0] + (cell.value[4] - cell.value[0]) * t;
		Bt = cell.value[3] + (cell.value[7] - cell.value[3]) * t;
		Ct = cell.value[2] + (cell.value[6] - cell.value[2]) * t;
		Dt = cell.value[1] + (cell.value[5] - cell.value[1]) * t;

		verify = (At * Ct) - (Bt * Dt);

		if (verify > 0)
			return 0;
		if (verify < 0)
			return 1;
		break;

	case 9:
		a = (cell.value[5] - cell.value[1]) * (cell.value[7] - cell.value[3])
			- (cell.value[4] - cell.value[0]) * (cell.value[6] - cell.value[2]);
		b = cell.value[3] * (cell.value[5] - cell.value[1]) + cell.value[1] * (cell.value[7] - cell.value[3])
			- cell.value[2] * (cell.value[4] - cell.value[0])
			- cell.value[0] * (cell.value[6] - cell.value[2]);
		if (a > 0)
			return 1;

		t = -b / static_cast<float>(2 * a);
		if (t < 0 || t > 1)
			return 1;

		At = cell.value[1] + (cell.value[5] - cell.value[1]) * t;
		Bt = cell.value[0] + (cell.value[4] - cell.value[0]) * t;
		Ct = cell.value[3] + (cell.value[7] - cell.value[3]) * t;
		Dt = cell.value[2] + (cell.value[6] - cell.value[2]) * t;

		verify = (At * Ct) - (Bt * Dt);

		if (verify > 0)
			return 0;
		if (verify < 0)
			return 1;
		break;

	case 10:
		a = (cell.value[6] - cell.value[2]) * (cell.value[4] - cell.value[0])
			- (cell.value[5] - cell.value[1]) * (cell.value[7] - cell.value[3]);
		b = cell.value[0] * (cell.value[6] - cell.value[2]) + cell.value[2] * (cell.value[4] - cell.value[0])
			- cell.value[3] * (cell.value[5] - cell.value[1])
			- cell.value[1] * (cell.value[7] - cell.value[3]);
		if (a > 0)
			return 1;

		t = -b / static_cast<float>(2 * a);
		if (t < 0 || t > 1)
			return 1;

		At = cell.value[2] + (cell.value[6] - cell.value[2]) * t;
		Bt = cell.value[1] + (cell.value[5] - cell.value[1]) * t;
		Ct = cell.value[0] + (cell.value[4] - cell.value[0]) * t;
		Dt = cell.value[3] + (cell.value[7] - cell.value[3]) * t;

		verify = (At * Ct) - (Bt * Dt);

		if (verify > 0)
			return 0;
		if (verify < 0)
			return 1;
		break;

	case 11:
		a = (cell.value[7] - cell.value[3]) * (cell.value[5] - cell.value[1])
			- (cell.value[6] - cell.value[2]) * (cell.value[4] - cell.value[0]);
		b = cell.value[1] * (cell.value[7] - cell.value[3]) + cell.value[3] * (cell.value[5] - cell.value[1])
			- cell.value[0] * (cell.value[6] - cell.value[2])
			- cell.value[2] * (cell.value[4] - cell.value[0]);
		if (a > 0)
			return 1;

		t = -b / static_cast<float>(2 * a);
		if (t < 0 || t > 1)
			return 1;

		At = cell.value[3] + (cell.value[7] - cell.value[3]) * t;
		Bt = cell.value[2] + (cell.value[6] - cell.value[2]) * t;
		Ct = cell.value[1] + (cell.value[5] - cell.value[1]) * t;
		Dt = cell.value[0] + (cell.value[4] - cell.value[0]) * t;

		verify = (At * Ct) - (Bt * Dt);

		if (verify > 0)
			return 0;
		if (verify < 0)
			return 1;
		break;
	}
	return 0;
}

bool Builder::interiorTestCase13()
{
	float t1, t2;
	float At1 = 0, Bt1 = 0, Ct1 = 0, Dt1 = 0;
	float At2 = 0, Bt2 = 0, Ct2 = 0, Dt2 = 0;
	float a = 0, b = 0, c = 0;

	a = (cell.value[0] - cell.value[1]) * (cell.value[7] - cell.value[6])
		- (cell.value[4] - cell.value[5]) * (cell.value[3] - cell.value[2]);
	b = cell.value[6] * (cell.value[0] - cell.value[1]) + cell.value[1] * (cell.value[7] - cell.value[6])
		- cell.value[2] * (cell.value[4] - cell.value[5])
		- cell.value[5] * (cell.value[3] - cell.value[2]);

	c = cell.value[1] * cell.value[6] - cell.value[5] * cell.value[2];

	float delta = b*b - 4 * a*c;

	t1 = (-b + sqrt(delta)) / static_cast<float>(2 * a);
	t2 = (-b - sqrt(delta)) / static_cast<float>(2 * a);

	cout << "t1 = " << t1 << " t2 = " << t2 << "\n";

	if ((t1 < 1) && (t1 > 0) && (t2 < 1) && (t2 > 0))
	{

		At1 = cell.value[1] + (cell.value[0] - cell.value[1]) * t1;
		Bt1 = cell.value[5] + (cell.value[4] - cell.value[5]) * t1;
		Ct1 = cell.value[6] + (cell.value[7] - cell.value[6]) * t1;
		Dt1 = cell.value[2] + (cell.value[3] - cell.value[2]) * t1;

		float x1 = (At1 - Dt1) / static_cast<float>(At1 + Ct1 - Bt1 - Dt1);
		float y1 = (At1 - Bt1) / static_cast<float>(At1 + Ct1 - Bt1 - Dt1);

		At2 = cell.value[1] + (cell.value[0] - cell.value[1]) * t2;
		Bt2 = cell.value[5] + (cell.value[4] - cell.value[5]) * t2;
		Ct2 = cell.value[6] + (cell.value[7] - cell.value[6]) * t2;
		Dt2 = cell.value[2] + (cell.value[3] - cell.value[2]) * t2;

		float x2 = (At2 - Dt2) / static_cast<float>(At2 + Ct2 - Bt2 - Dt2);
		float y2 = (At2 - Bt2) / static_cast<float>(At2 + Ct2 - Bt2 - Dt2);

		if ((x1 < 1) && (x1 > 0) && (x2 < 1) && (x2 > 0) && (y1 < 1) && (y1 > 0) && (y2 < 1) && (y2 > 0))
			return false;

		else
			return true;
	}

	else 
		return true;
}