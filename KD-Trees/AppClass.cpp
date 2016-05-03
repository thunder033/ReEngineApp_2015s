#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("3D Transformations");
	m_v4ClearColor = vector4(0.0f, 0.0f, 0.0f, 0.0f);
}

void AppClass::InitVariables(void)
{
	pointCount = 20;
	minBounds = vector3(-5);
	maxBounds = vector3(5);

	for (size_t i = 0; i < pointCount; i++)
	{
		float x = minBounds.x + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxBounds.x - minBounds.x)));
		float y = minBounds.y + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxBounds.y - minBounds.y)));
		float z = minBounds.z + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxBounds.z - minBounds.z)));

		points.push_back(vector3(x, y, z));
	}

	tree = new kDTree(5, points);
}

void AppClass::Update(void)
{
#pragma region Does not need changes

	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();

	//First person camera movement
	if (m_bFPC == true)
		CameraRotation();

	//Call the arcball method
	ArcBall();

	//This matrices will just orient the objects to the camera
	matrix4 rotateX = glm::rotate(IDENTITY_M4, 90.0f, vector3(1.0f, 0.0f, 0.0f));
	matrix4 rotateY = glm::rotate(IDENTITY_M4, 90.0f, vector3(0.0f, 1.0f, 0.0f));

#pragma endregion

#pragma region YOUR CODE GOES HERE
	
	
#pragma endregion

#pragma region Print info
	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
#pragma endregion

	//renamed from earth/moon timers
	m_fFrames++;
	m_fTimeElapsed = m_fFrames / m_fSimulationSpeed;
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();

	for (size_t i = 0; i < pointCount; i++) {
		m_pMeshMngr->AddSphereToRenderList(glm::translate(points[i]) * glm::scale(vector3(0.1f)), REYELLOW, SOLID);
	}

	vector3 genBounds = maxBounds - minBounds;
	m_pMeshMngr->AddCubeToRenderList(glm::scale(genBounds), REWHITE, WIRE);

	tree->Draw(minBounds, maxBounds);
	
	//Render the grid based on the camera's mode:
	//m_pMeshMngr->AddGridToRenderListBasedOnCamera(m_pCameraMngr->GetCameraMode());
	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ResetRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{

	super::Release(); //release the memory of the inherited fields
}