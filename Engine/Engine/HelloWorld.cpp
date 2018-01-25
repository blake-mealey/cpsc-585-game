#include "Engine/Systems/Graphics.h"
#include "Engine/Systems/Physics.h"
#include "Engine/Components/InputManager.h"
#include "Engine/Systems/Content/ContentManager.h"

#include "Engine/Systems/IO/XboxController.h"

#define _USE_MATH_DEFINES
#include <math.h>
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

	// Load the scene and get some entities
	ContentManager::LoadScene("GraphicsDemo.json");
	Entity *boulder = EntityManager::FindEntities("Boulder")[0];
	Entity *camera = EntityManager::FindEntities("Camera")[0];

	const int lightCount = 10;
	for (int i = 0; i < lightCount; i++) {
		Entity *entity = EntityManager::CreateStaticEntity();

		const float angle = i * ((2 * M_PI) / lightCount);
		entity->transform.SetPosition(6.f * glm::vec3(sin(angle), 0, cos(angle)));
		entity->transform.SetScale(glm::vec3(0.1f));

		const glm::vec3 color = glm::vec3(unitRand(), unitRand(), unitRand());

		PointLightComponent *light = new PointLightComponent(color, 20);
		EntityManager::AddComponent(entity, light);
		
		MeshComponent *mesh = new MeshComponent("Sphere.obj", new Material(color, color, 1));
		EntityManager::AddComponent(entity, mesh);
	}

//	camera->transform.SetPosition(glm::vec3(0, 5.f, -10.f));

	InputManager inputManager;
	
	//New Controller Stuff
	XboxController* player;
	//New Controller Stuff
	player = new XboxController(1);

	//Game Loop
	while (!glfwWindowShouldClose(graphicsManager.GetWindow())) {

		if (player->IsConnected()) {
			if (player->GetState().Gamepad.bLeftTrigger > 5) {
				player->Vibrate(30000, 65535);
			}
			else {
				player->Vibrate();
			}
		}

		//Calculate Delta Time
		Time currentTime = glfwGetTime();
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

		inputManager.Update();
	}
}
