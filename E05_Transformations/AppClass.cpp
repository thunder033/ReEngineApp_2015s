#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Bobadilla, Alberto - 3D Transformations");
	m_v4ClearColor = vector4(0.0f, 0.0f, 0.0f, 0.0f);
}

matrix4 translate(matrix4 mat, vector4 vec) {
	mat[3] = vec;
	return mat;
}

matrix4 scale(matrix4 mat, float scalar) {
	mat[0, 0] *= scalar;
	mat[1, 1] *= scalar;
	mat[2, 2] *= scalar;
	return mat;
}

void AppClass::InitVariables(void)
{
	//Sets the camera
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 0.0f, 15.0f), vector3(0.0f, 0.0f, 0.0f), REAXISY);

	m_pCube = new PrimitiveClass();
	m_pCone = new PrimitiveClass();
	m_pCylinder = new PrimitiveClass();
	m_pTube = new PrimitiveClass();
	m_pSphere = new PrimitiveClass();
	m_pTorus = new PrimitiveClass();

	//Initializing the primitives
	m_pSphere->GenerateSphere(0.5f, 5, REWHITE);

	//m_m4Sphere = IDENTITY_M4;
	m_m4Sphere = glm::translate(IDENTITY_M4, vector3(1, 2, 0));
	m_m4Sphere = glm::scale(m_m4Sphere, vector3(2, 2, 2));
}

void AppClass::Update(void)
{
	//This matrices will just place the objects int the right spots
	//m_m4Sphere = glm::translate(IDENTITY_M4, vector3(2.5f, 2.5f, 0.0f));

	//This matrices will scale them to the right size
	//m_m4Sphere = glm::scale(m_m4Sphere, vector3(2.0f, 2.0f, 2.0f));

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();
	m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XY); //renders the XY grid with a 100% scale

	//Matrices from the camera
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();

	//Renders the meshes using the specified position given by the matrix and in the specified color
	m_pSphere->Render(m4Projection, m4View, m_m4Sphere);

	//To render the render list (right now it only contains the grid)
	m_pMeshMngr->Render();
	
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	SafeDelete(m_pCube);
	SafeDelete(m_pCone);
	SafeDelete(m_pCylinder);
	SafeDelete(m_pTube);
	SafeDelete(m_pSphere);
	SafeDelete(m_pTorus);

	super::Release(); //release the memory of the inherited fields
}