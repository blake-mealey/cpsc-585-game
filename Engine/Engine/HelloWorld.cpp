#include "Engine/Systems/Graphics.h"
#include "Engine/Systems/Physics.h"
#include "Engine/Systems/InputManager.h"
#include "Engine/Systems/Content/ContentManager.h"

#include "Engine/Systems/IO/XboxController.h"

#include <vector>
#include <iostream>
#include "Engine/Entities/Entity.h"
#include "Engine/Entities/EntityManager.h"
#include "Engine/Components/MeshComponent.h"
#pragma comment(lib, "opengl32.lib")

using namespace std;

float unitRand() {
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

int main() {
	cout << "Hello, World!" << endl;

	Time currentTime;
	Time lastFrame(0);
	Time deltaTime(0);

	vector<System*> systems;

	// Initialize graphics and add to systems vector
	Graphics &graphicsManager = Graphics::Instance();
	graphicsManager.Initialize("Car Wars");
	systems.push_back(&graphicsManager);

	// Initialize physics and add to systems vector
	Physics &physicsManager = Physics::Instance();
	systems.push_back(&physicsManager);

	// Initialize inputManager and add to systems vector
	InputManager &inputManager = InputManager::Instance();
	systems.push_back(&inputManager);

	// Load the scene and get some entities
	ContentManager::LoadScene("GraphicsDemo.json");
	Entity *boulder = EntityManager::FindEntities("Boulder")[0];
	Entity *camera = EntityManager::FindEntities("Camera")[0];

	//Game Loop
	while (!glfwWindowShouldClose(graphicsManager.GetWindow())) {

		//Calculate Delta Time
		currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		// "Game" logic
		boulder->transform.SetPosition(glm::vec3(0*cos(currentTime.GetTimeMilliSeconds() / 500), sin(currentTime.GetTimeMilliSeconds()/500), 0));
		boulder->transform.Rotate(glm::vec3(1, 1, 1), deltaTime.GetTimeMilliSeconds() * 0.00002);
		camera->transform.SetPosition(10.f * glm::vec3(
			sin(currentTime.GetTimeMilliSeconds() / 1000), 0.5,
			cos(currentTime.GetTimeMilliSeconds() / 1000)));

		// Iterate through each system and call their update methods
		for (auto it = systems.begin(); it != systems.end(); ++it) {
			(*it)->Update(deltaTime);
		}
	}
}
