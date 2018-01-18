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

	Graphics *graphicsManager = Graphics::Instance();
	graphicsManager->Initialize("Game Title");
	systems.push_back(graphicsManager);

	Physics *physicsManager = Physics::Instance();
	systems.push_back(physicsManager);

	Entity *camera = EntityManager::CreateDynamicEntity();
	CameraComponent cameraComponent = CameraComponent();
	EntityManager::AddComponent(camera, &cameraComponent);
	EntityManager::AddComponent(camera, &cameraComponent);

	Material *material = ContentManager::GetMaterial("Basic.json");
	Entity *boulder = EntityManager::CreateDynamicEntity();
	MeshComponent boulderMesh = MeshComponent("Boulder.objm", material, "Boulder.jpg");
	boulderMesh.transform.SetRotationAxisAngles(glm::vec3(0, 1, 0), 3.14*0.5);
	EntityManager::AddComponent(boulder, &boulderMesh);

	Entity *floor = EntityManager::CreateStaticEntity();
	floor->transform.SetPosition(glm::vec3(0, -2, 0));
	MeshComponent floorMesh = MeshComponent("Plane.objm", material, "DiamondPlate.jpg");
	EntityManager::AddComponent(floor , &floorMesh);

	InputManager inputManager;
	
	float angle = 0;

	//Game Loop
	while (!glfwWindowShouldClose(graphicsManager->GetWindow())) {
		//Calculate Delta Time
		Time currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		// "Game" logic
		boulder->transform.SetPosition(glm::vec3(0*cos(currentTime.GetTimeMilliSeconds() / 500), sin(currentTime.GetTimeMilliSeconds()/500), 0));
		boulder->transform.Rotate(glm::vec3(1, 1, 1), deltaTime.GetTimeMilliSeconds() * 0.00002);
		cameraComponent.SetPosition(5.f * glm::vec3(sin(angle), 0, cos(angle += 0.01)));

		// Iterate through each system and call their update methods
		for (vector<System*>::iterator it = systems.begin(); it != systems.end(); ++it) {
			(*it)->Update(deltaTime);
		}

		inputManager.Update();
	}
}
