#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Assignment  06 - LERP"); // Window Name
}

void AppClass::InitVariables(void)
{
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 0.0f, 25.0f), ZERO_V3, REAXISY);

	// Color of the screen
	m_v4ClearColor = vector4(REBLACK, 1); // Set the clear color to black

	m_pMeshMngr->LoadModel("Sorted\\WallEye.bto", "WallEye");

	//distance (meters?) per second
	fSpeed = 1.0f;

	m_nObjects = 11;
	nLastTarget = m_nObjects - 1;
	m_path = new vector3[11] {
		vector3(-4.0f, -2.0f, 5.0f),
		vector3(1.0f, -2.0f, 5.0f),
		vector3(-3.0f, -1.0f, 3.0f),
		vector3(2.0f, -1.0f, 3.0f),
		vector3(-2.0f, 0.0f, 0.0f),
		vector3(3.0f, 0.0f, 0.0f),
		vector3(-1.0f, 1.0f, -3.0f),
		vector3(4.0f, 1.0f, -3.0f),
		vector3(0.0f, 2.0f, -5.0f),
		vector3(5.0f, 2.0f, -5.0f),
		vector3(1.0f, 3.0f, -5.0f),
	};

	m_pMatrix = new glm::mat4[m_nObjects];
	m_pSphere = new PrimitiveClass[m_nObjects];

	for (size_t i = 0; i < m_nObjects; i++)
	{
		m_pSphere[i] = PrimitiveClass();
		m_pSphere[i].GenerateSphere(0.1f, 12, vector3(1, 0, 0));
		m_pMatrix[i] *= glm::translate(IDENTITY_M4, m_path[i]);
	}
}

void AppClass::Update(void)
{
#pragma region Does not change anything here
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();
#pragma region

#pragma region Does not need changes but feel free to change anything here
	//Lets us know how much time has passed since the last call
	double fDeltaTime = m_pSystem->LapClock(); //Delta time (between frame calls)

	//cumulative time
	static double fRunTime = 0.0f; //How much time has passed since the program started
	fRunTime += fDeltaTime; 
#pragma endregion

#pragma region Your Code goes here
	float fPct = fDistTraveled / fTargetDist;
	fDistTraveled += fSpeed * fDeltaTime;

	if (fPct > 1)
	{
		fDistTraveled = 0;
		fPct = 0;
		nLastTarget = nTarget;
		nTarget = (nTarget + 1) % m_nObjects;

		vector3 d = (m_path[nLastTarget] - m_path[nTarget]);
		fTargetDist = sqrt((d.x * d.x) + (d.y * d.y) + (d.z * d.z));
	}

	float x = MapValue(fPct, 0.0f, 1.0f, m_path[nLastTarget].x, m_path[nTarget].x);
	float y = MapValue(fPct, 0.0f, 1.0f, m_path[nLastTarget].y, m_path[nTarget].y);
	float z = MapValue(fPct, 0.0f, 1.0f, m_path[nLastTarget].z, m_path[nTarget].z);
	m_pMeshMngr->SetModelMatrix(glm::translate(IDENTITY_M4, vector3(x, y, z)), "WallEye");
#pragma endregion

#pragma region Does not need changes but feel free to change anything here
	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
#pragma endregion
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();

	//Render the grid based on the camera's mode:
	switch (m_pCameraMngr->GetCameraMode())
	{
	default: //Perspective
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XY); //renders the XY grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOX:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::YZ, RERED * 0.75f); //renders the YZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOY:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XZ, REGREEN * 0.75f); //renders the XZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOZ:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XY, REBLUE * 0.75f); //renders the XY grid with a 100% scale
		break;
	}

	for (size_t i = 0; i < m_nObjects; i++)
	{
		m_pSphere[i].Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m_pMatrix[i]);
	}

	m_pMeshMngr->Render(); //renders the render list

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	delete[] m_pSphere;
	delete[] m_pMatrix;
	delete[] m_path;

	super::Release(); //release the memory of the inherited fields
}