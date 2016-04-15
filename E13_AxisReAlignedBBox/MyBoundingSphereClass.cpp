#include "MyBoundingSphereClass.h"
//  MyBoundingSphereClass
void MyBoundingSphereClass::Init(void)
{
	m_fRadius = 0.0f;
	m_m4ToWorld = IDENTITY_M4;

	m_v3Center = vector3(0.0f);
	m_v3Min = vector3(0.0f);
	m_v3Max = vector3(0.0f);
}
void MyBoundingSphereClass::Swap(MyBoundingSphereClass& other)
{
	std::swap(m_fRadius, other.m_fRadius);
	std::swap(m_m4ToWorld, other.m_m4ToWorld);

	std::swap(m_v3Center, other.m_v3Center);
	std::swap(m_v3Min, other.m_v3Min);
	std::swap(m_v3Max, other.m_v3Max);
}
void MyBoundingSphereClass::Release(void)
{

}

void MyBoundingSphereClass::GetMinMax(vector3& min, vector3& max, std::vector<vector3> points) {
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
MyBoundingSphereClass::MyBoundingSphereClass(std::vector<vector3> a_lVectorList)
{
	GetMinMax(m_v3Min, m_v3Max, a_lVectorList);
	m_v3Center = (m_v3Min + m_v3Max) / 2.0f;
	m_fRadius = glm::distance(m_v3Center, m_v3Max);
}
MyBoundingSphereClass::MyBoundingSphereClass(MyBoundingSphereClass const& other)
{
	m_fRadius = other.m_fRadius;
	m_m4ToWorld = other.m_m4ToWorld;

	m_v3Center = other.m_v3Center;
	m_v3Min = other.m_v3Min;
	m_v3Max = other.m_v3Max;
}
MyBoundingSphereClass& MyBoundingSphereClass::operator=(MyBoundingSphereClass const& other)
{
	if(this != &other)
	{
		Release();
		Init();
		MyBoundingSphereClass temp(other);
		Swap(temp);
	}
	return *this;
}
MyBoundingSphereClass::~MyBoundingSphereClass(){Release();};
//Accessors
void MyBoundingSphereClass::SetModelMatrix(matrix4 a_m4ToWorld){ m_m4ToWorld = a_m4ToWorld; }
vector3 MyBoundingSphereClass::GetCenter(void){ return vector3(m_m4ToWorld * vector4(m_v3Center, 1.0f)); }
float MyBoundingSphereClass::GetRadius(void) { return m_fRadius; }
//--- Non Standard Singleton Methods
bool MyBoundingSphereClass::IsColliding(MyBoundingSphereClass* const a_pOther)
{
	float dist = glm::distance(GetCenter(), a_pOther->GetCenter());
	return dist < (GetRadius() + a_pOther->GetRadius());
}