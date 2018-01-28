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
#include "Engine/Components/SpotLightComponent.h"
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

//	static_cast<MeshComponent*>(EntityManager::FindEntities("Floor")[0]->components[0])->texture = new Texture(graphicsManager.textureIds[Textures::Depth], 1024, 1024);

	/*const int lightCount = 5;
	for (int i = 0; i < lightCount; ++i) {
		Entity *entity = EntityManager::CreateStaticEntity();

		const float angle = i * ((2 * M_PI) / lightCount);
		const glm::vec3 position = 6.f * glm::vec3(sin(angle), 0, cos(angle));
		entity->transform.SetPosition(position);
		entity->transform.SetScale(glm::vec3(0.1f));

		const glm::vec3 color = glm::vec3(unitRand(), unitRand(), unitRand());

//		PointLightComponent *light = new PointLightComponent(color, 10);
		SpotLightComponent *light = new SpotLightComponent(color, 20, glm::radians(20.f), -position - glm::vec3(0, 2, 0));
		EntityManager::AddComponent(entity, light);
		
		MeshComponent *mesh = new MeshComponent("Sphere.obj", new Material(color, color, 1));
		EntityManager::AddComponent(entity, mesh);
	}*/

	camera->transform.SetPosition(glm::vec3(-5, 5, 0));

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
//		camera->transform.SetPosition(10.f * glm::vec3(
//			sin(currentTime.GetTimeMilliSeconds() / 1000), 0.5,
//			cos(currentTime.GetTimeMilliSeconds() / 1000)));

		camera->transform.SetPosition(glm::vec3(-5.f, 5, 1 * sin(currentTime.GetTimeMilliSeconds() / 1000)));

		// Iterate through each system and call their update methods
		for (auto it = systems.begin(); it != systems.end(); ++it) {
			(*it)->Update(deltaTime);
		}

		inputManager.Update();
	}
}
