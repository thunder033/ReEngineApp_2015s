#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("SLERP - GREG ROZMARYNOWYCZ"); // Window Name

	//Setting the color to black
	m_v4ClearColor = vector4(0.0f);
}

void AppClass::InitVariables(void)
{
	//Setting the position in which the camera is looking and its interest point
	m_pCameraMngr->SetPositionTargetAndView(vector3(12.12f, 32.52f, 11.34f), ZERO_V3, REAXISY);

	//Setting the color to black
	m_v4ClearColor = vector4(0.0f);

	//Loading the models
	m_pMeshMngr->LoadModel("Planets\\00_Sun.obj", "Sun");
	m_pMeshMngr->LoadModel("Planets\\02_Venus.obj", "Venus");
	m_pMeshMngr->LoadModel("Planets\\03_Earth.obj", "Earth");
	m_pMeshMngr->LoadModel("Planets\\03A_Moon.obj", "Moon");

	//Setting the days duration
	m_fDay = 1.0f;
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

	//Getting the time between calls
	double fCallTime = m_pSystem->LapClock();
	//Counting the cumulative time
	static double dTimeElapsed = 0.0f;
	dTimeElapsed += fCallTime;

	//Earth Orbit
	double fEarthHalfOrbTime = 182.5f * m_fDay; //Earths orbit around the sun lasts 365 days / half the time for 2 stops
	float fEarthHalfRevTime = 0.5f * m_fDay; // Move for Half a day
	float fMoonHalfOrbTime = 14.0f * m_fDay; //Moon's orbit is 28 earth days, so half the time for half a route

	//This matrices will just orient the objects to the camera
	matrix4 rotateX = glm::rotate(IDENTITY_M4, 90.0f, vector3(1.0f, 0.0f, 0.0f));
	matrix4 rotateY = glm::rotate(IDENTITY_M4, 90.0f, vector3(0.0f, 1.0f, 0.0f));

	//This matrices will hold the relative transformation of the Moon and the Earth
	matrix4 distanceEarth = glm::translate(15.0f, 0.0f, 0.0f);
	matrix4 distanceVenus = glm::translate(10.0f, 0.0f, 0.0f);
	matrix4 distanceMoon = glm::translate(2.0f, 0.0f, 0.0f);

	/*
	I know celestial accuracy wasn't really the point but this was much easier for me to understand with things more accurate
	The solar rotation and earth axial tilt are extra but they were easy to add once I figured everything out
	*/

	float daysInYear = 365.25f;
	float daysPerLunarYear = 27.0f; //also the same as one lunar rotation/day
	float daysPerSolarRotation = 24.47f;
	float fTimeElapsed = static_cast<float>(dTimeElapsed);

	float daysInVenusYear = 224.7f;
	float daysPerVenusDay = 116.75f;

	vector3 vXUnit = vector3(1, 0, 0);
	vector3 vYUnit = vector3(0, 1, 0);
	vector3 vZUnit = vector3(0, 0, 1);

	quaternion rotBegin = quaternion(vector3());
	quaternion rotEnd = quaternion(vector3(0, 6.28f, 0));

	//Calculate the position of the Earth
	m_m4Sun = glm::translate(IDENTITY_M4, glm::vec3(m_m4Sun[3])); //persist the sun's posisition only
	m_m4Earth = m_m4Sun; //move the earth & venus back to the sun
	m_m4Venus = m_m4Sun;

	quaternion earthOrbit = glm::mix(rotBegin, rotEnd, fTimeElapsed / (daysInYear * m_fDay));
	m_m4Earth *= glm::mat4_cast(earthOrbit); //rotate the earth based on orbit position
	m_m4Earth *= distanceEarth; //translate to orbit position

	quaternion sunRot = glm::mix(rotBegin, rotEnd, fTimeElapsed / (daysPerSolarRotation * m_fDay));
	m_m4Sun *= glm::mat4_cast(sunRot) * glm::scale(vector3(1) * 5.936f); //apply rotation to sun

	//yay venus
	quaternion venusRot = glm::mix(rotBegin, rotEnd, fTimeElapsed / (daysInVenusYear * m_fDay));
	m_m4Venus *= glm::mat4_cast(venusRot) * distanceVenus * glm::scale(vector3(1) * 0.499f);

	//Calculate the position of the Moon
	quaternion moonRot = glm::mix(rotBegin, rotEnd, fTimeElapsed / (daysPerLunarYear * m_fDay));
	m_m4Moon = glm::translate(IDENTITY_M4, glm::vec3(m_m4Earth[3])) * glm::mat4_cast(moonRot); //move moon to earth and apply orbital rotation
	m_m4Moon *= distanceMoon * glm::scale(vector3(1) * 0.27f); //translate moon out to its orbit, tidally lock it to the earth

	quaternion earthRot = glm::mix(rotBegin, rotEnd, fTimeElapsed / m_fDay); //rotate for day/night cycle
	//preserve only the position of the earth after it is translated
	m_m4Earth = glm::translate(IDENTITY_M4, glm::vec3(m_m4Earth[3])) * glm::mat4_cast(earthRot)
		* glm::scale(vector3(1) * 0.525f);

	//Setting the matrices
	m_pMeshMngr->SetModelMatrix(m_m4Sun, "Sun");
	m_pMeshMngr->SetModelMatrix(m_m4Venus, "Venus");
	m_pMeshMngr->SetModelMatrix(m_m4Earth, "Earth");
	m_pMeshMngr->SetModelMatrix(m_m4Moon, "Moon");

	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	int nEarthOrbits = fTimeElapsed / (daysInYear * m_fDay);
	int nEarthRevolutions = fTimeElapsed / m_fDay;
	int nMoonOrbits = fTimeElapsed / (daysPerLunarYear * m_fDay);

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	
	m_pMeshMngr->Print("Time:");
	m_pMeshMngr->PrintLine(std::to_string(dTimeElapsed));

	m_pMeshMngr->Print("Day:");
	m_pMeshMngr->PrintLine(std::to_string(m_fDay));

	m_pMeshMngr->Print("E_Orbits:");
	m_pMeshMngr->PrintLine(std::to_string(nEarthOrbits));

	m_pMeshMngr->Print("E_Revolutions:");
	m_pMeshMngr->PrintLine(std::to_string(nEarthRevolutions));

	m_pMeshMngr->Print("M_Orbits:");
	m_pMeshMngr->PrintLine(std::to_string(nMoonOrbits));

	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();

	//Render the grid based on the camera's mode:
	switch (m_pCameraMngr->GetCameraMode())
	{
	default:
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
}