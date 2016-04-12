#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("MyBoundingSphereClass example"); // Window Name

	// Set the clear color based on Microsoft's CornflowerBlue (default in XNA)
	//if this line is in Init Application it will depend on the .cfg file, if it
	//is on the InitVariables it will always force it regardless of the .cfg
	m_v4ClearColor = vector4(0.4f, 0.6f, 0.9f, 0.0f);
}

void getMinMax(vector3& min, vector3& max, std::vector<vector3> points) {
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

void AppClass::InitVariables(void)
{
	//Initialize positions
	m_v3O1 = vector3(-2.5f, 0.0f, 0.0f);
	m_v3O2 = vector3(2.5f, 0.0f, 0.0f);

	//Load Models
	m_pMeshMngr->LoadModel("Minecraft\\Steve.obj", "Steve");
	m_pMeshMngr->LoadModel("Minecraft\\Creeper.obj", "Creeper");

	std::vector<vector3> points = m_pMeshMngr->GetVertexList("Steve");
	vector3 min, max;

	getMinMax(min, max, points);
	steveCenter = (min + max) / 2.0f;
	steveRadius = glm::distance(steveCenter, max);

	points = m_pMeshMngr->GetVertexList("Creeper");
	getMinMax(min, max, points);
	creeperCenter = (min + max) / 2.0f;
	creeperRadius = glm::distance(creeperCenter, max);

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

	vector3 v3SteveGlob = vector3(m4Steve * vector4(steveCenter, 1.0f));
	vector3 v3CreeperGlob = vector3(m4Creeper * vector4(creeperCenter, 1.0f));

	float dist = glm::distance(v3SteveGlob, v3CreeperGlob);
	bool areColliding = dist < (steveRadius + creeperRadius);

	m_pMeshMngr->Print("x: " + std::to_string(v3SteveGlob.x) + " ", RERED);
	m_pMeshMngr->Print("y: " + std::to_string(v3SteveGlob.y) + " ", RERED);
	m_pMeshMngr->Print("z: " + std::to_string(v3SteveGlob.z) + " ", RERED);
	m_pMeshMngr->PrintLine("");

	vector3 color = areColliding ? RERED : REGREEN;
	m4Steve = m_pMeshMngr->GetModelMatrix("Steve") * glm::translate(steveCenter);
	m_pMeshMngr->AddSphereToQueue(m4Steve * glm::scale(vector3(steveRadius * 2.0f)), color, WIRE);

	m4Creeper = m_pMeshMngr->GetModelMatrix("Creeper") * glm::translate(creeperCenter);
	m_pMeshMngr->AddSphereToQueue(m4Creeper * glm::scale(vector3(creeperRadius * 2.0f)), color, WIRE);

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

	//matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	//matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();

	//matrix4 m4Model = m_pMeshMngr->GetModelMatrix("Steve") * glm::translate(steveCenter) * glm::scale(vector3(steveRadius * 2.0f));
	////m_pSphere1->Render(m4Projection,m4View, m4Model);
	//m_pMeshMngr->AddSphereToQueue(m4Model, RERED, WIRE);

	//m4Model = m_pMeshMngr->GetModelMatrix("Creeper") * glm::translate(creeperCenter)* glm::scale(vector3(creeperRadius * 2.0f));
	////m_pSphere2->Render(m4Projection, m4View, m4Model);
	//m_pMeshMngr->AddSphereToQueue(m4Model, RERED, WIRE);
	
	m_pMeshMngr->Render(); //renders the render list

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields

	if (m_pSphere1 != nullptr)
	{
		delete m_pSphere1;
		m_pSphere1 = nullptr;
	}
}