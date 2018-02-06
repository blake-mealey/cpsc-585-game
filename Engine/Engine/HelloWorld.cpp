#include "Engine/Systems/Graphics.h"
#include "Engine/Systems/Physics.h"
#include "Engine/Systems/InputManager.h"

#define _USE_MATH_DEFINES
#include <vector>
#include "Engine/Systems/Game.h"
#pragma comment(lib, "opengl32.lib")

using namespace std;

int main() {
	vector<System*> systems;
	
	// Initialize systems
	
	// Initialize graphics (MUST come before entity instantiation)
	Graphics &graphicsManager = Graphics::Instance();
	graphicsManager.Initialize("Car Wars");
	
	// Initialize input
	InputManager &inputManager = InputManager::Instance();
	
	// Initialize physics
	Physics &physicsManager = Physics::Instance();
    physicsManager.Initialize();
	
	// Initialize game
	Game &gameManager = Game::Instance();
	gameManager.Initialize();

	// Add systems in desired order
	systems.push_back(&inputManager);
	systems.push_back(&physicsManager);
	systems.push_back(&gameManager);
	systems.push_back(&graphicsManager);

	//Game Loop
	Time lastFrame(0);
	while (!glfwWindowShouldClose(graphicsManager.GetWindow())) {

		//Calculate Delta Time
		Time currentTime = glfwGetTime();
		const Time deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		// Iterate through each system and call their update methods
		for (System* system : systems) {
			system->Update(currentTime, deltaTime);
		}
	}
}
