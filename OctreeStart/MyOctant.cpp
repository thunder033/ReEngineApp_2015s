#include "MyOctant.h"
using namespace ReEng;
//  MyOctant
void MyOctant::Init(void)
{
	m_v3Position = vector3(0.0f);
	m_pMeshMngr = MeshManagerSingleton::GetInstance();
	m_fSize = 0.0f;
}
void MyOctant::Swap(MyOctant& other)
{

}
void MyOctant::Release(void)
{
}
//The big 3
MyOctant::MyOctant(){Init();}
MyOctant::MyOctant(float a_fSize) { Init(); m_fSize = a_fSize; }
MyOctant::MyOctant(MyOctant const& other)
{

}
MyOctant& MyOctant::operator=(MyOctant const& other)
{
	if(this != &other)
	{
		Release();
		Init();
		MyOctant temp(other);
		Swap(temp);
	}
	return *this;
}
MyOctant::~MyOctant(){Release();};

void MyOctant::Display(void)
{
	m_pMeshMngr->AddCubeToRenderList(glm::translate(m_v3Position) * glm::scale(vector3(m_fSize)), REWHITE, WIRE);
}

void MyOctant::Subdivide(void)
{
	m_pChildren = new MyOctant[8];
	for (uint index = 0; index < 8; index++)
	{
		m_pChildren[index].m_fSize = this->m_fSize / 2;

	}
}
void MyOctant::ReleaseChildren(void)
{
	if (m_pChildren != nullptr)
	{
		delete[] m_pChildren;
		m_pChildren = nullptr;
	}
}
//Accessors

//--- Non Standard Singleton Methods

