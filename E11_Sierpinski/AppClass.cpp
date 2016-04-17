#include "AppClass.h"

void AppClass::InitWindow(String a_sWindowName)
{
	//Using Base InitWindow method
	super::InitWindow("Instance Rendering");
}

void AppClass::AddTransformToRenderList(matrix4 transform)
{
	/*std::cout << "Adding point";
	const float* m4MVP = glm::value_ptr(transform);
	memcpy(&m_fMatrixArray[1 * m_iMatrixSize], m4MVP, m_iMatrixSize * sizeof(float));*/
}


void AppClass::InitVariables(void)
{
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 0.0f, 15.0f), vector3(0.0f, 0.0f, 0.0f), REAXISY);

	m_pMesh = new MyMesh();
	
	//Creating the Mesh points
	m_pMesh->AddVertexPosition(vector3(-1.0f, -1.0f, 0.0f));
	m_pMesh->AddVertexColor(REGREEN);
	m_pMesh->AddVertexPosition(vector3( 1.0f, -1.0f, 0.0f));
	m_pMesh->AddVertexColor(RERED);
	m_pMesh->AddVertexPosition(vector3(0.0f,  1.0f, 0.0f));
	m_pMesh->AddVertexColor(REBLUE);

	//Compiling the mesh
	m_pMesh->CompileOpenGL3X();

	//const int rows = 15;
	//const int pointsCount = 136;
	//int* points = new int[pointsCount] {
	//	1,
	//	1, 1,
	//	1, 2, 1,
	//	1, 3, 3, 1,
	//	1, 4, 6, 4, 1,
	//	1, 5, 10, 10, 5, 1,
	//	1, 6, 15, 20, 15, 6, 1,
	//	1, 7, 21, 35, 35, 21, 7, 1,
	//	1, 8, 28, 56, 70, 56, 28, 8, 1,
	//	1, 9, 36, 84, 126, 126, 84, 36, 9, 1,
	//	1, 10, 45, 120, 210, 242, 210, 120, 45, 10, 1,
	//	1, 11, 55, 165, 330, 452, 452, 330, 165, 55, 11, 1,
	//	1, 12, 66, 220, 495, 782, 904, 782, 495, 220, 66, 12, 1,
	//	1, 13, 78, 286, 715, 1287, 1716, 1716, 1287, 715, 286, 78, 13, 1,
	//	1, 14, 91, 364, 1001, 2002, 3003, 3432, 3003, 2002, 1001, 364, 91, 14, 1,
	//	1, 15, 105, 455, 1365, 3003, 5005, 6435, 6435, 5005, 3003, 1365, 455, 105, 15, 1
	//};

	//The number of triangles drawn for a S. Triangle of a given size is (at most) the number of rows squared
	m_fMatrixArray = new float[1 * 16];

	//matrix4 mat4 = IDENTITY_M4 * glm::translate(vector3(0, 0, 0));
	//AddTransformToRenderList(IDENTITY_M4);
	
	const float* m4MVP = glm::value_ptr(IDENTITY_M4);
	memcpy(&m_fMatrixArray[0], m4MVP, 16 * sizeof(float));

	/*for (int i = 0; i < pointsCount; i++) {
		if (points[i] % 2 == 1) {
			
		}
	}*/

	for (int u = 0; u < 16; u++) {
		std::cout << std::to_string(m_fMatrixArray[u]) + (" ");
		if((u + 1 ) % 4 == 0)
			std::cout << std::endl;
	}
}

void AppClass::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->UpdateTime();

	//Is the arcball active?
	if (m_bArcBall == true)
		ArcBall();

	//Is the first person camera active?
	if (m_bFPC == true)
		CameraRotation();

	//Calculate Camera
	m_pCameraMngr->CalculateView();

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	//print info into the console
	printf("FPS: %d            \r", nFPS);//print the Frames per Second
										  //Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the window

	//Matrices from the camera
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();

	m_pMesh->RenderList(m4Projection, m4View, m_fMatrixArray, 16);
	//m_pMesh->Render(m4Projection, m4View, IDENTITY_M4);//Rendering nObjects

	m_pMeshMngr->AddSphereToQueue(IDENTITY_M4, RERED, WIRE);
	m_pMeshMngr->Render();
	

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	SafeDelete(m_fMatrixArray);
	if (m_pMesh != nullptr)
	{
		delete m_pMesh;
		m_pMesh = nullptr;
	}
	super::Release();
}