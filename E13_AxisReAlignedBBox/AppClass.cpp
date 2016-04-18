#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("E13 Axis-Re-aligned Bounding Box"); // Window Name

	// Set the clear color based on Microsoft's CornflowerBlue (default in XNA)
	//if this line is in Init Application it will depend on the .cfg file, if it
	//is on the InitVariables it will always force it regardless of the .cfg
	m_v4ClearColor = vector4(0.4f, 0.6f, 0.9f, 0.0f);
}

void AppClass::InitVariables(void)
{
	//Initialize positions
	m_v3O1 = vector3(-2.5f, 0.0f, 0.0f);
	m_v3O2 = vector3(2.5f, 0.0f, 0.0f);

	//Load Models
	m_pMeshMngr->LoadModel("Minecraft\\Steve.obj", "Steve");
	m_pMeshMngr->LoadModel("Minecraft\\Creeper.obj", "Creeper");

	m_pColSteve = new BoundingBox(m_pMeshMngr->GetVertexList("Steve"));
	m_pColCreeper = new BoundingBox(m_pMeshMngr->GetVertexList("Creeper"));

}

void AppClass::Update(void)
{
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();

	//First person camera movement
	if (m_bFPC == true)
		CameraRotation();

	ArcBall();

	//Set the model matrices for both objects and Bounding Spheres
	m_pMeshMngr->SetModelMatrix(glm::translate(m_v3O1) * ToMatrix4(m_qArcBall), "Steve");
	m_pMeshMngr->SetModelMatrix(glm::translate(m_v3O2), "Creeper");

	m_pColSteve->SetModelMatrix(m_pMeshMngr->GetModelMatrix("Steve"));
	std::vector<vector3> box = m_pColSteve->Rotate(m_qArcBall);
	m_pColCreeper->SetModelMatrix(m_pMeshMngr->GetModelMatrix("Creeper"));

	vector3 color = m_pColSteve->IsColliding(m_pColCreeper) ? RERED : REGREEN;

	m_pMeshMngr->AddCubeToQueue(
		glm::translate(m_pColSteve->GetCenter()) * m_pColSteve->GetRot() * glm::scale(m_pColSteve->GetSize()),
		color, WIRE);
	m_pMeshMngr->AddCubeToQueue(
		m_pColSteve->GetAxisAlignedTransform(),
		color, WIRE);
	m_pMeshMngr->AddSphereToQueue(glm::translate(m_pColSteve->GetMin()) * glm::scale(vector3(0.15f)), RERED, SOLID);
	m_pMeshMngr->AddSphereToQueue(glm::translate(m_pColSteve->GetMax()) * glm::scale(vector3(0.15f)), RERED, SOLID);

	for (int i = 0; i < 8; i++)
	{
		m_pMeshMngr->AddSphereToQueue(glm::translate(vector3(m_pMeshMngr->GetModelMatrix("Steve")[3] + vector4(box[i], 1.0f))) * glm::scale(vector3(0.05f)), REBLUE, SOLID);
	}

	m_pMeshMngr->AddCubeToQueue(
		glm::translate(m_pColCreeper->GetCenter()) * glm::scale(m_pColCreeper->GetSize()),
		color, WIRE);

	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//print info into the console
	printf("FPS: %d            \r", nFPS);//print the Frames per Second
	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
	m_pMeshMngr->PrintLine("");
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
	
	m_pMeshMngr->Render(); //renders the render list

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields

	if (m_pColSteve != nullptr)
	{
		delete m_pColSteve;
		m_pColSteve = nullptr;
	}

	if (m_pColCreeper != nullptr)
	{
		delete m_pColCreeper;
		m_pColCreeper = nullptr;
	}
}