#include "BoundingBox.h"

//  BoundingBox
void BoundingBox::Init(void)
{
	m_fRadius = 0.0f;
	m_m4ToWorld = IDENTITY_M4;

	m_v3Center = vector3(0.0f);
	m_v3Min = vector3(0.0f);
	m_v3Max = vector3(0.0f);
}
void BoundingBox::Swap(BoundingBox& other)
{
	std::swap(m_fRadius, other.m_fRadius);
	std::swap(m_m4ToWorld, other.m_m4ToWorld);

	std::swap(m_v3Center, other.m_v3Center);
	std::swap(m_v3Min, other.m_v3Min);
	std::swap(m_v3Max, other.m_v3Max);
}
void BoundingBox::Release(void)
{

}

void BoundingBox::GetMinMax(vector3& min, vector3& max, std::vector<vector3> points) {
	min = points[0];
	max = points[0];

	std::vector<vector3>::iterator it;
	for (it = points.begin(); it != points.end(); ++it)
	{
		if (it->x < min.x)
			min.x = it->x;
		else if (it->x > max.x)
			max.x = it->x;

		if (it->y < min.y)
			min.y = it->y;
		else if (it->y > max.y)
			max.y = it->y;

		if (it->z < min.z)
			min.z = it->z;
		else if (it->z > max.z)
			max.z = it->z;
	}
}

//The big 3
BoundingBox::BoundingBox(std::vector<vector3> a_lVectorList)
{
	GetMinMax(m_v3Min, m_v3Max, a_lVectorList);
	m_v3Center = (m_v3Min + m_v3Max) / 2.0f;

	m_fRadius = glm::distance(m_v3Center, m_v3Max);

	m_vSize.x = m_v3Max.x - m_v3Min.x;
	m_vSize.y = m_v3Max.y - m_v3Min.y;
	m_vSize.z = m_v3Max.z - m_v3Min.z;
	m_vAlignedSize = m_vSize;
}
BoundingBox::BoundingBox(BoundingBox const& other)
{
	m_fRadius = other.m_fRadius;
	m_m4ToWorld = other.m_m4ToWorld;

	m_v3Center = other.m_v3Center;
	m_v3Min = other.m_v3Min;
	m_v3Max = other.m_v3Max;
}
BoundingBox& BoundingBox::operator=(BoundingBox const& other)
{
	if(this != &other)
	{
		Release();
		Init();
		BoundingBox temp(other);
		Swap(temp);
	}
	return *this;
}
BoundingBox::~BoundingBox(){Release();};
//Accessors
void BoundingBox::SetModelMatrix(matrix4 a_m4ToWorld){ m_m4ToWorld = a_m4ToWorld; }
vector3 BoundingBox::GetCenter(void){ return vector3(m_m4ToWorld * vector4(m_v3Center, 1.0f)); }
float BoundingBox::GetRadius(void) { return m_fRadius; }
std::vector<vector3> BoundingBox::Rotate(quaternion rot)
{
	m_qRotation = rot;

	float fValue = 0.5f;
	//3--2
	//|  |
	//0--1
	std::vector<vector3> box{
		vector3(-fValue, -fValue, fValue), //0
		vector3(fValue, -fValue, fValue), //1
		vector3(fValue, fValue, fValue), //2
		vector3(-fValue, fValue, fValue), //3

		vector3(-fValue, -fValue, -fValue), //4
		vector3(fValue, -fValue, -fValue), //5
		vector3(fValue, fValue, -fValue), //6
		vector3(-fValue, fValue, -fValue) //7
	};

	for (int i = 0; i < 8; i++) {
		box[i] = vector3(ToMatrix4(m_qRotation) * glm::translate(m_v3Center) * glm::scale(m_vSize) * vector4(box[i], 1));
	}

	GetMinMax(m_v3Min, m_v3Max, box);
	m_vAlignedSize.x = m_v3Max.x - m_v3Min.x;
	m_vAlignedSize.y = m_v3Max.y - m_v3Min.y;
	m_vAlignedSize.z = m_v3Max.z - m_v3Min.z;

	return box;
}

matrix4 BoundingBox::GetAxisAlignedTransform()
{
	return glm::translate(GetCenter()) * glm::scale(m_vAlignedSize);
}

vector3 BoundingBox::GetSize(void) { return m_vSize; }
matrix4 BoundingBox::GetRot(void)
{
	return ToMatrix4(m_qRotation);
}


vector3 BoundingBox::GetMin() {
	return vector3(m_m4ToWorld[3] + vector4(m_v3Min, 1.0f));
}

vector3 BoundingBox::GetMax() {
	return vector3(m_m4ToWorld[3] + vector4(m_v3Max, 1.0f));
}

//--- Non Standard Singleton Methods
bool BoundingBox::IsColliding(BoundingBox* const a_pOther)
{
	//from Hammer (monogame #c engine)
	//return (position.X <= collidee.position.X + collidee.bBox.X &&
	//	collidee.position.X <= position.X + bBox.X &&
	//	position.Y <= collidee.position.Y + collidee.bBox.Y &&
	//	collidee.position.Y <= position.Y + bBox.Y);

	vector3 v3Min = GetMin();
	vector3 v3MinO = a_pOther->GetMin();

	vector3 v3Max = GetMax();
	vector3 v3MaxO = a_pOther->GetMax();

	return !(v3Min.x > v3MaxO.x || v3MinO.x > v3Max.x ||
		v3Min.y > v3MaxO.y || v3MinO.y > v3Max.y ||
		v3Min.z > v3MaxO.z || v3MinO.z > v3Max.z);
}