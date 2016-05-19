#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("3D Transformations");
	m_v4ClearColor = vector4(0.0f, 0.0f, 0.0f, 0.0f);
}

void AppClass::InitVariables(void)
{
	m_m4Sun = IDENTITY_M4;
	m_m4Earth = IDENTITY_M4;
	m_m4Moon = IDENTITY_M4;

	m_pSun = new PrimitiveClass();
	m_pEarth = new PrimitiveClass();
	m_pMoon = new PrimitiveClass();

	m_pSun->GenerateSphere(5.936f, 5, REYELLOW);
	m_pEarth->GenerateTube(0.524f, 0.45f, 0.3f, 10, REBLUE);
	m_pMoon->GenerateTube(0.524f * 0.27f, 0.45f * 0.27f, 0.3f * 0.27f, 10, REWHITE);
}

void AppClass::Update(void)
{
#pragma region Does not need changes
	//Sets the camera
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 25.0f, 0.0f), vector3(0.0f, 0.0f, 0.0f), -REAXISZ);

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

	//This matrices will hold the relative transformation of the Moon and the Earth
	matrix4 distanceEarth = glm::translate(11.0f, 0.0f, 0.0f);
	matrix4 distanceMoon = glm::translate(2.0f, 0.0f, 0.0f);
#pragma endregion

#pragma region YOUR CODE GOES HERE
	
	/*
	I know celestial accuracy wasn't really the point but this was much easier for me to understand with things more accurate
	The solar rotation and earth axial tilt are extra but they were easy to add once I figured everything out
	*/

	float daysInYear = 365.25f;
	float daysPerLunarYear = 27.0f; //also the same as one lunar rotation/day
	float daysPerSolarRotation = 24.47f;
	float earthAxialTilt = 23.4;

	vector3 vXUnit = vector3(1, 0, 0);
	vector3 vYUnit = vector3(0, 1, 0);
	vector3 vZUnit = vector3(0, 0, 1);

	//Calculate the position of the Earth
	m_m4Sun = glm::translate(IDENTITY_M4, glm::vec3(m_m4Sun[3])); //persist the sun's posisition only
	m_m4Earth = m_m4Sun; //move the earth back to the sun
	m_m4Earth = glm::rotate(m_m4Earth, m_fTimeElapsed, vYUnit); //rotate the earth based on orbit position
	m_m4Earth *= distanceEarth; //translate to orbit position

	m_m4Sun *= glm::rotate(IDENTITY_M4, m_fTimeElapsed * (daysInYear / daysPerSolarRotation), vYUnit); //apply rotation to sun

	//Calculate the position of the Moon
	m_m4Moon = glm::rotate(m_m4Earth, m_fTimeElapsed * (daysInYear/daysPerLunarYear), vYUnit); //move moon to earth and apply orbital rotation
	m_m4Moon *= distanceMoon * rotateX * glm::rotate(IDENTITY_M4, 90.0f, vZUnit); //translate moon out to its orbit, tidally lock it to the earth

	m_m4Earth *= glm::rotate(IDENTITY_M4, -m_fTimeElapsed, vYUnit) //negate orbital Y rotation of the earth because it's axial tilt isn't tidally locked to sun
		* glm::rotate(IDENTITY_M4, 90.0f - earthAxialTilt, vXUnit)  //apply axial tilt to the earth
		* glm::rotate(IDENTITY_M4, -m_fTimeElapsed * daysInYear, vZUnit); //rotate for day/night cycle

#pragma endregion

#pragma region Print info
	printf("Earth Day: %.3f, Moon Day: %.3f\r", m_fTimeElapsed, m_fFrames);//print the Frames per Second
	
	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("Earth Day: ", REWHITE);
	m_pMeshMngr->PrintLine(std::to_string((m_fTimeElapsed * daysInYear)/360.0f), REBLUE);
	m_pMeshMngr->Print("Lunar Year: ", REWHITE);
	m_pMeshMngr->PrintLine(std::to_string((m_fTimeElapsed * (daysInYear / daysPerLunarYear))/360.0f), REBLUE);
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

	//Renders the meshes using the specified position given by the matrix and in the specified color
	m_pSun->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m_m4Sun);
	m_pEarth->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m_m4Earth);
	m_pMoon->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m_m4Moon);
	
	//Render the grid based on the camera's mode:
	m_pMeshMngr->AddGridToRenderListBasedOnCamera(m_pCameraMngr->GetCameraMode());
	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	SafeDelete(m_pEarth);
	SafeDelete(m_pMoon);
	SafeDelete(m_pSun);

	super::Release(); //release the memory of the inherited fields
}