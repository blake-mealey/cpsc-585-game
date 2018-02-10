#include "Game.h"

#include <iostream>
#include "Content/ContentManager.h"
#include "../Entities/EntityManager.h"
#include "../Components/SpotLightComponent.h"
#include "../Components/MeshComponent.h"
#include "../Components/CameraComponent.h"

#include "Physics\VehicleCreate.h"
#include "Physics\VehicleWheelQueryResult.h"
#include "Physics\SnippetUtils.h"
#include "Physics\VehicleConcurrency.h"
#include "Physics\VehicleFilterShader.h"
#include "Physics\VehicleSceneQuery.h"
#include "Physics\VehicleTireFriction.h"

#include <glm/gtx/string_cast.hpp>

#define _USE_MATH_DEFINES
#include <math.h>
#include "StateManager.h"
using namespace std;

const unsigned int Game::MAX_VEHICLE_COUNT = 8;

Time gameTime(0);

// Singleton
Game::Game() {}
Game &Game::Instance() {
	static Game instance;
	return instance;
}

float unitRand() {
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

void Game::Initialize() {
    ContentManager::LoadSkybox("PurpleNebula/");

	ContentManager::LoadScene("PhysicsDemo.json");

	cars = EntityManager::FindEntities("Vehicle");
	cameras = EntityManager::FindEntities("Camera");

	for (Entity* camera : cameras) {
		glm::vec3 pos = 20.0f * glm::vec3(cos(-3.14 / 2) * sin(3.14 / 4), cos(3.14 / 4), sin(-3.14 / 2) * sin(3.14 / 4));
		static_cast<CameraComponent*>(camera->components[0])->SetPosition(pos);
	}

	// Load the scene and get some entities
	/*ContentManager::LoadScene("GraphicsDemo.json");
	boulder = EntityManager::FindEntities("Boulder")[0];
	sun = EntityManager::FindEntities("Sun")[0];
	floor = EntityManager::FindEntities("Floor")[0];
	baby = EntityManager::FindEntities("Baby")[0];

	//Create Vehicle
	const PxU32 numWheels = 4;

	PxVehicleWheelsSimData* wheelsSimData = PxVehicleWheelsSimData::allocate(numWheels);
	//setupWheelsSimluationData(wheelsSimData);



	//camera->transform.SetPosition(glm::vec3(0, 5, 10));

	const int lightCount = 5;
	for (int i = 0; i < lightCount; ++i) {
		Entity *entity = EntityManager::CreateStaticEntity();

		const float angle = i * ((2 * M_PI) / lightCount);
		const glm::vec3 position = 6.f * glm::vec3(sin(angle), 0, cos(angle));
		entity->transform.SetPosition(position);
		entity->transform.SetScale(glm::vec3(0.1f));

		const glm::vec3 color = glm::vec3(unitRand(), unitRand(), unitRand());

		//PointLightComponent *light = new PointLightComponent(color, 10);
		SpotLightComponent *light = new SpotLightComponent(color, 20, glm::radians(20.f), -position - glm::vec3(0, 2, 0));
		EntityManager::AddComponent(entity, light);

		MeshComponent *mesh = new MeshComponent("Sphere.obj", new Material(color, color, 1));
		EntityManager::AddComponent(entity, mesh);
	}*/
}

void Game::Update(Time currentTime, Time deltaTime) {
	if (StateManager::GetState() == GameState_Playing) {
		gameTime += deltaTime;

		//boulder->transform.Translate(glm::vec3(0.0f, sin(currentTime.GetTimeSeconds()), 0.0f));
	    //const glm::vec3 pos = boulder->transform.GetLocalPosition();
        //boulder->transform.SetPosition(glm::vec3(pos.x, sin(gameTime.GetTimeMilliSeconds() / 500), pos.z));
		//boulder->transform.Rotate(glm::vec3(0, 1, 0), deltaTime.GetTimeMilliSeconds() * 0.00002);

		//boulder->transform.Translate(boulder->transform.GetForward() * 0.1f);

		//camera->transform.SetPosition(10.f * glm::vec3(
			//sin(gameTime.GetTimeMilliSeconds() / 1000), 0.5,
			//cos(gameTime.GetTimeMilliSeconds() / 1000)));
		
		for (int i = 0; i < cars.size(); i++) {
			Entity* camera = cameras[i];
			Entity* car = cars[i];

			//camera->transform.SetPosition(glm::mix(camera->transform.GetGlobalPosition(), boulder->transform.GetGlobalPosition(), 0.05f));
			camera->transform.SetPosition(glm::mix(camera->transform.GetGlobalPosition(), car->transform.GetGlobalPosition(), 0.04f));
			//static_cast<CameraComponent*>(camera->components[0])->SetTarget(boulder->transform.GetGlobalPosition());
			static_cast<CameraComponent*>(camera->components[0])->SetTarget(car->transform.GetGlobalPosition() + glm::vec3(0.0f, 1.0f, 0.0f));
		}

		//camera->transform.SetPosition(boulder->transform.GetGlobalPosition());

		//floor->transform.Rotate({ 0,0,1 }, deltaTime.GetTimeMilliSeconds() * 0.00002);
		//baby->transform.Translate(glm::vec3(.01, 0, 0));
		//baby->transform.Rotate(glm::vec3(1,0,0),.01f);

	} else if (StateManager::GetState() == GameState_Paused) {
		std::cout << "paused" << std::endl;
	}
}