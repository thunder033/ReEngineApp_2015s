#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("MyBoundingSphereClass example"); // Window Name

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

	m_pColSteve = new MyBoundingSphereClass(m_pMeshMngr->GetVertexList("Steve"));
	m_pColCreeper = new MyBoundingSphereClass(m_pMeshMngr->GetVertexList("Creeper"));

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

	//m_pMeshMngr->Print("x: " + std::to_string(v3SteveGlob.x) + " ", RERED);
	//m_pMeshMngr->Print("y: " + std::to_string(v3SteveGlob.y) + " ", RERED);
	//m_pMeshMngr->Print("z: " + std::to_string(v3SteveGlob.z) + " ", RERED);
	//m_pMeshMngr->PrintLine("");

	std::cout << m_pColSteve->GetRadius() << std::endl;
	std::cout << m_pColCreeper->GetRadius() << std::endl;

	m_pColSteve->SetModelMatrix(m_pMeshMngr->GetModelMatrix("Steve"));
	m_pColCreeper->SetModelMatrix(m_pMeshMngr->GetModelMatrix("Creeper"));

	vector3 color = m_pColSteve->IsColliding(m_pColCreeper) ? RERED : REGREEN;

	//m4Steve = m_pMeshMngr->GetModelMatrix("Steve") * glm::translate(steveCenter);
	//m_pMeshMngr->AddSphereToQueue(m4Steve * glm::scale(vector3(steveRadius * 2.0f)), color, WIRE);

	//m4Creeper = m_pMeshMngr->GetModelMatrix("Creeper") * glm::translate(creeperCenter);
	//m_pMeshMngr->AddSphereToQueue(m4Creeper * glm::scale(vector3(creeperRadius * 2.0f)), color, WIRE);

	m_pMeshMngr->AddSphereToQueue(
		glm::translate(m_pColSteve->GetCenter()) * glm::scale(vector3(m_pColSteve->GetRadius() * 2.0f)), 
		color, WIRE);
	m_pMeshMngr->AddSphereToQueue(
		glm::translate(m_pColCreeper->GetCenter()) * glm::scale(vector3(m_pColCreeper->GetRadius() * 2.0f)),
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

	//matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	//matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();

	//matrix4 m4Model = m_pMeshMngr->GetModelMatrix("Steve") * glm::translate(steveCenter) * glm::scale(vector3(steveRadius * 2.0f));
	////m_pSphere1->Render(m4Projection,m4View, m4Model);
	//m_pMeshMngr->AddSphereToQueue(m4Model, RERED, WIRE);

	//m4Model = m_pMeshMngr->GetModelMatrix("Creeper") * glm::translate(creeperCenter)* glm::scale(vector3(creeperRadius * 2.0f));
	////m_pSphere2->Render(m4Projection, m4View, m4Model);
	//m_pMeshMngr->AddSphereToQueue(m4Model, RERED, WIRE);

	m_pMeshMngr->AddGridToRenderListBasedOnCamera(m_pCameraMngr->GetCameraMode());

	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
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