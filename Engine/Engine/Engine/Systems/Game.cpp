#include "Game.h"

#include <iostream>
#include "Content/ContentManager.h"
#include "../Entities/EntityManager.h"
#include "../Components/SpotLightComponent.h"
#include "../Components/MeshComponent.h"

#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;

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

	// Load the scene and get some entities
	ContentManager::LoadScene("GraphicsDemo.json");
	boulder = EntityManager::FindEntities("Boulder")[0];
	camera = EntityManager::FindEntities("Camera")[0];
	sun = EntityManager::FindEntities("Sun")[0];
	floor = EntityManager::FindEntities("Floor")[0];

	camera->transform.SetPosition(glm::vec3(0, 5, 10));

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
	}
}

void Game::Update(Time currentTime, Time deltaTime) {
	boulder->transform.SetPosition(glm::vec3(0 * cos(currentTime.GetTimeMilliSeconds() / 500), sin(currentTime.GetTimeMilliSeconds() / 500), 0));
	boulder->transform.Rotate(glm::vec3(1, 1, 1), deltaTime.GetTimeMilliSeconds() * 0.00002);
	camera->transform.SetPosition(10.f * glm::vec3(
		sin(currentTime.GetTimeMilliSeconds() / 1000), 0.5,
		cos(currentTime.GetTimeMilliSeconds() / 1000)));
	floor->transform.Rotate({ 0,0,1 }, deltaTime.GetTimeMilliSeconds() * 0.00002);
}