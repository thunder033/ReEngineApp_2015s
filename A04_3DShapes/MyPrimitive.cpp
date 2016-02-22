#include "MyPrimitive.h"
MyPrimitive::MyPrimitive() { }
MyPrimitive::MyPrimitive(const MyPrimitive& other) { }
MyPrimitive& MyPrimitive::operator=(const MyPrimitive& other) { return *this; }
MyPrimitive::~MyPrimitive(void) { super::Release(); }

void MyPrimitive::CompileObject(vector3 a_v3Color)
{
	m_uVertexCount = static_cast<int> (m_lVertexPos.size());
	for (uint i = 0; i < m_uVertexCount; i++)
	{
		AddVertexColor(a_v3Color);
	}
	
	CompleteTriangleInfo(true);
	CompileOpenGL3X();

}
//C--D
//|\ |
//| \|
//A--B
//This will make the triang A->B->C and then the triang C->B->D
void MyPrimitive::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}
void MyPrimitive::GeneratePlane(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = 0.5f * a_fSize;

	vector3 pointA(-fValue, -fValue, 0.0f); //0
	vector3 pointB(fValue, -fValue, 0.0f); //1
	vector3 pointC(fValue, fValue, 0.0f); //2
	vector3 pointD(-fValue, fValue, 0.0f); //3

	vector3 pointE(fValue, -fValue, -0.001f); //1
	vector3 pointF(-fValue, -fValue, -0.001f); //0
	vector3 pointG(fValue, fValue, -0.001f); //2
	vector3 pointH(-fValue, fValue, -0.001f); //3

											  //F
	AddQuad(pointA, pointB, pointD, pointC);
	//Double sided
	AddQuad(pointE, pointF, pointG, pointH);

	CompileObject(a_v3Color);
}
vector<vector3> MyPrimitive::GenerateNGon(float a_fRadius, int a_nSides, vector3 a_v3Offset)
{
	vector<vector3> points = vector<vector3>();

	float increment = (3.14159 * 2) / a_nSides;
	for (size_t i = 0; i < a_nSides; i++)
	{
		float x = a_fRadius * cos(i * increment) + a_v3Offset[0];
		float y = a_fRadius * sin(i * increment) + a_v3Offset[1];
		float z = a_v3Offset[2];
		points.push_back(vector3(x, y, z));
	}

	return points;
}
void MyPrimitive::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = 0.5f * a_fSize;
	//3--2
	//|  |
	//0--1
	vector3 point0(-fValue, -fValue, fValue); //0
	vector3 point1(fValue, -fValue, fValue); //1
	vector3 point2(fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue, -fValue, -fValue); //4
	vector3 point5(fValue, -fValue, -fValue); //5
	vector3 point6(fValue, fValue, -fValue); //6
	vector3 point7(-fValue, fValue, -fValue); //7

											  //F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//Your code starts here
	vector<vector3> basePoints = GenerateNGon(a_fRadius, a_nSubdivisions, vector3(0, 0, -a_fHeight / 2));
	vector3 vertexPoint = vector3(0, 0, a_fHeight / 2);
	vector3 baseCenter = vector3(0, 0, -a_fHeight / 2);

	for (size_t i = 0; i < a_nSubdivisions; i++)
	{
		AddVertexPosition(basePoints[i]);
		AddVertexPosition(basePoints[(i + 1) % a_nSubdivisions]);
		AddVertexPosition(vertexPoint);

		AddVertexPosition(baseCenter);
		AddVertexPosition(basePoints[(i + 1) % a_nSubdivisions]);
		AddVertexPosition(basePoints[i]);
	}
	

	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//Your code starts here
	vector3 base1Center = vector3(0, 0, -a_fHeight / 2);
	vector<vector3> base1Points = GenerateNGon(a_fRadius, a_nSubdivisions, base1Center);

	vector3 base2Center = vector3(0, 0, a_fHeight / 2);
	vector<vector3> base2Points = GenerateNGon(a_fRadius, a_nSubdivisions, base2Center);

	for (size_t i = 0; i < a_nSubdivisions; i++)
	{
		int rightIndex = i;
		int leftIndex = (i + 1) % a_nSubdivisions;

		AddVertexPosition(base1Center);
		AddVertexPosition(base1Points[leftIndex]);
		AddVertexPosition(base1Points[rightIndex]);

		AddQuad(base2Points[leftIndex], base2Points[rightIndex], base1Points[leftIndex], base1Points[rightIndex]);

		AddVertexPosition(base2Points[rightIndex]);
		AddVertexPosition(base2Points[leftIndex]);
		AddVertexPosition(base2Center);
	}

	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//Your code starts here
	vector3 base1Center = vector3(0, 0, -a_fHeight / 2);
	vector<vector3> base1InnerPoints = GenerateNGon(a_fInnerRadius, a_nSubdivisions, base1Center);
	vector<vector3> base1OuterPoints = GenerateNGon(a_fOuterRadius, a_nSubdivisions, base1Center);

	vector3 base2Center = vector3(0, 0, a_fHeight / 2);
	vector<vector3> base2InnerPoints = GenerateNGon(a_fInnerRadius, a_nSubdivisions, base2Center);
	vector<vector3> base2OuterPoints = GenerateNGon(a_fOuterRadius, a_nSubdivisions, base2Center);

	for (size_t i = 0; i < a_nSubdivisions; i++)
	{
		int rightIndex = i;
		int leftIndex = (i + 1) % a_nSubdivisions;

		AddQuad(base2InnerPoints[rightIndex], base2InnerPoints[leftIndex], base1InnerPoints[rightIndex], base1InnerPoints[leftIndex]);
		AddQuad(base2OuterPoints[leftIndex], base2OuterPoints[rightIndex], base1OuterPoints[leftIndex], base1OuterPoints[rightIndex]);

		AddQuad(base2OuterPoints[rightIndex], base2OuterPoints[leftIndex], base2InnerPoints[rightIndex], base2InnerPoints[leftIndex]);
		AddQuad(base1OuterPoints[leftIndex], base1OuterPoints[rightIndex], base1InnerPoints[leftIndex], base1InnerPoints[rightIndex]);
	}

	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius <= a_fInnerRadius + 0.1f)
		return;

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 25)
		a_nSubdivisionsA = 25;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 25)
		a_nSubdivisionsB = 25;

	Release();
	Init();

	//Your code starts here
	//Your code starts here
	
	

	float ringRadius = (a_fOuterRadius - a_fInnerRadius) / 2;
	float radIncrement = (3.1459 * 2) / (a_nSubdivisionsB);

	float z = ringRadius * cos(a_nSubdivisionsB * radIncrement);
	float xRadius = ringRadius * sin(a_nSubdivisionsB * radIncrement);
	vector3 base1Center = vector3(0, 0, z);
	vector<vector3> lastRingPoints = GenerateNGon(a_fInnerRadius + ringRadius + xRadius, a_nSubdivisionsA, base1Center);

	for (int i = a_nSubdivisionsB - 1; i > -1; i--)
	{
		float z = ringRadius * cos(i * radIncrement);
		float xRadius = ringRadius * sin(i * radIncrement);

		vector<vector3> ringPoints = GenerateNGon(a_fInnerRadius + ringRadius + xRadius, a_nSubdivisionsA, vector3(0, 0, z));

		for (size_t j = 0; j < a_nSubdivisionsA; j++)
		{
			int rightIndex = j;
			int leftIndex = (j + 1) % a_nSubdivisionsA;

			AddQuad(lastRingPoints[rightIndex], lastRingPoints[leftIndex], ringPoints[rightIndex], ringPoints[leftIndex]);
		}

		lastRingPoints = ringPoints;
	}

	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
	{
		GenerateCube(a_fRadius * 2, a_v3Color);
		return;
	}
	if (a_nSubdivisions > 12)
		a_nSubdivisions = 12;

	Release();
	Init();

	//Your code starts here
	vector3 base1Center = vector3(0, 0, 0);
	vector<vector3> centerRingPoints = GenerateNGon(a_fRadius, a_nSubdivisions, base1Center);
	vector<vector3> prevUpperRingPoints = centerRingPoints;
	vector<vector3> prevLowerRingPoints = centerRingPoints;

	vector3 topCenter = vector3(0, 0, a_fRadius);
	vector3 bottomCenter = vector3(0, 0, -a_fRadius);

	float radIncrement = (3.1459 / 2) / (a_nSubdivisions / 2);
	for (size_t i = a_nSubdivisions / 2; i > 0; i--)
	{
		float z = a_fRadius * cos(i * radIncrement);
		float xRadius = a_fRadius * sin(i * radIncrement);

		vector<vector3> upperRingPoints = GenerateNGon(xRadius, a_nSubdivisions, vector3(0, 0, z));
		vector<vector3> lowerRingPoints = GenerateNGon(xRadius, a_nSubdivisions, vector3(0, 0, -z));

		for (size_t j = 0; j < a_nSubdivisions; j++)
		{
			int rightIndex = j;
			int leftIndex = (j + 1) % a_nSubdivisions;

			AddQuad(prevUpperRingPoints[rightIndex], prevUpperRingPoints[leftIndex], upperRingPoints[rightIndex], upperRingPoints[leftIndex]);
			AddQuad(prevLowerRingPoints[leftIndex], prevLowerRingPoints[rightIndex], lowerRingPoints[leftIndex], lowerRingPoints[rightIndex]);

			if (i == 1) {
				AddVertexPosition(upperRingPoints[rightIndex]);
				AddVertexPosition(upperRingPoints[leftIndex]);
				AddVertexPosition(topCenter);

				AddVertexPosition(bottomCenter);
				AddVertexPosition(lowerRingPoints[leftIndex]);
				AddVertexPosition(lowerRingPoints[rightIndex]);
				
			}
		}

		prevUpperRingPoints = upperRingPoints;
		prevLowerRingPoints = lowerRingPoints;
	}


	//Your code ends here
	CompileObject(a_v3Color);
}