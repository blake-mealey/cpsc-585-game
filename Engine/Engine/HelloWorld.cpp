#include "Engine/Systems/Graphics.h"
#include "Engine/Systems/Physics.h"
#include "Engine/Components/InputManager.h"
#include "Engine/Systems/Content/ContentManager.h"

#include <vector>
#include <iostream>
#include "Engine/Entities/Entity.h"
#include "Engine/Entities/EntityManager.h"
#pragma comment(lib, "opengl32.lib")

using namespace std;

int main() {
	cout << "Hello, World!" << endl;

	Time lastFrame(0);
	Time deltaTime(0);

	vector<System*> systems;

	// Initialize graphics and add to systems vector
	Graphics *graphicsManager = Graphics::Instance();
	graphicsManager->Initialize("Game Title");
	systems.push_back(graphicsManager);

	// Initialize physics and add to systems vector
	Physics *physicsManager = Physics::Instance();
	systems.push_back(physicsManager);

	// Load the scene and get some entities
	ContentManager::LoadScene("Level.json");
	Entity *boulder = EntityManager::FindEntities("Boulder")[0];
	Entity *camera = EntityManager::FindEntities("Camera")[0];

	InputManager inputManager;
	
	//Game Loop
	while (!glfwWindowShouldClose(graphicsManager->GetWindow())) {
		//Calculate Delta Time
		Time currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		// "Game" logic
		boulder->transform.SetPosition(glm::vec3(0*cos(currentTime.GetTimeMilliSeconds() / 500), sin(currentTime.GetTimeMilliSeconds()/500), 0));
		boulder->transform.Rotate(glm::vec3(1, 1, 1), deltaTime.GetTimeMilliSeconds() * 0.00002);
		camera->transform.SetPosition(5.f * glm::vec3(sin(currentTime.GetTimeMilliSeconds() / 1000), 0, cos(currentTime.GetTimeMilliSeconds() / 1000)));

		// Iterate through each system and call their update methods
		for (vector<System*>::iterator it = systems.begin(); it != systems.end(); ++it) {
			(*it)->Update(deltaTime);
		}

		inputManager.Update();
	}
}
