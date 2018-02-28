#include "AppClass.h"
void Application::InitVariables(void)
{
	//Change this to your name and email
	m_sProgrammer = "Alberto Bobadilla - labigm@rit.edu";
	
	//Set the position and target of the camera
	//(I'm at [0,0,10], looking at [0,0,0] and up is the positive Y axis)
	m_pCameraMngr->SetPositionTargetAndUp(AXIS_Z * 20.0f, ZERO_V3, AXIS_Y);

	//if the light position is zero move it
	if (m_pLightMngr->GetPosition(1) == ZERO_V3)
		m_pLightMngr->SetPosition(vector3(0.0f, 0.0f, 3.0f));

	//if the background is cornflowerblue change it to black (its easier to see)
	if (vector3(m_v4ClearColor) == C_BLUE_CORNFLOWER)
	{
		m_v4ClearColor = vector4(ZERO_V3, 1.0f);
	}
	
	//if there are no segments create 7
	if(m_uOrbits < 1)
		m_uOrbits = 7;

	float fSize = 1.0f; //initial size of orbits

	//creating a color using the spectrum 
	uint uColor = 650; //650 is Red
	//prevent division by 0
	float decrements = 250.0f / (m_uOrbits > 1? static_cast<float>(m_uOrbits - 1) : 1.0f); //decrement until you get to 400 (which is violet)
	/*
		This part will create the orbits, it start at 3 because that is the minimum subdivisions a torus can have
	*/
	uint uSides = 3; //start with the minimal 3 sides
	for (uint i = uSides; i < m_uOrbits + uSides; i++)
	{
		vector3 v3Color = WaveLengthToRGB(uColor); //calculate color based on wavelength
		m_shapeList.push_back(m_pMeshMngr->GenerateTorus(fSize, fSize - 0.1f, 3, i, v3Color)); //generate a custom torus and add it to the meshmanager
		fSize += 0.5f; //increment the size for the next orbit
		uColor -= static_cast<uint>(decrements); //decrease the wavelength
	}
	float pi = 3.141592654f;
	fSize = 1.0f; //reset radius

	//loop through toruses
	for (uint i = 0; i < m_uOrbits; i++) {
		std::vector<vector3> tempVecList; //makes temporarily list of stops for current torus

		//loop through the number of stops we'll need;
		for (uint b = 0; b < i + 3; b++) {
			float x = glm::cos(pi * (2.0f / (i + 3)) * b) * fSize; //x position of stop b
			float y = glm::sin(pi * (2.0f / (i + 3)) * b) * fSize; //y position of stop b
			tempVecList.push_back(vector3(x, y, 0)); //push new stop to templist
		}
		m_stopList.push_back(tempVecList); //push temp list (list of stops for torus i) to master list
		fSize += 0.5f; //increase radius for next time around
	}
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	//Get a timer
	static float fTimer = 0;	//store the new timer
	static uint uClock = m_pSystem->GenClock(); //generate a new clock for that timer
	fTimer += m_pSystem->GetDeltaTime(uClock); //get the delta time for that timer

	// Clear the screen
	ClearScreen();

	matrix4 m4View = m_pCameraMngr->GetViewMatrix(); //view Matrix
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix(); //Projection Matrix
	matrix4 m4Offset = IDENTITY_M4; //offset of the orbits, starts as the global coordinate system
	/*
		The following offset will orient the orbits as in the demo, start without it to make your life easier.
	*/
	//m4Offset = glm::rotate(IDENTITY_M4, 90.0f, AXIS_Z);

	//set up traversal of stops
	static float lastTime = 0; //the clock time that we hit the previous node
	static int currentNode = 0; //the current node we're heading towards

	float fPercent = fTimer - lastTime; //find the current percent of the way our ball should be towards the next node

	//check to see if percentage needs a reset
	if (fPercent > 1) {
		currentNode++; //increment to next node in list
		fPercent = 0; //reset percent
		lastTime = fTimer; //set the time we hit this node
	}

	// draw a shapes
	for (uint i = 0; i < m_uOrbits; ++i)
	{
		m_pMeshMngr->AddMeshToRenderList(m_shapeList[i], glm::rotate(m4Offset, 90.0f, AXIS_X));

		//calculate the current position
		vector3 oldNode = m_stopList[i][currentNode % (i+3)]; //start node
		vector3 newNode = m_stopList[i][(currentNode + 1) % (i+3)]; //end node
		vector3 v3CurrentPos = glm::lerp(oldNode, newNode, fPercent); //current position given start, end, and percentage way to travel
		matrix4 m4Model = glm::translate(m4Offset, v3CurrentPos); //get mat4 from offset and currentPos

		//draw spheres
		m_pMeshMngr->AddSphereToRenderList(m4Model * glm::scale(vector3(0.1)), C_WHITE);
	}

	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//release GUI
	ShutdownGUI();
}