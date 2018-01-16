#include "Engine/Systems/Graphics.h"
#include "Engine/Systems/Physics.h"
#include "Engine/Components/InputManager.h"

#include <vector>

using namespace std;

int main() {
	cout << "Hello, World!" << endl;

	//Delta Time Variables
	Time currentFrame;
	Time lastFrame(0);
	Time deltaTime(0);

	vector<System*> systems;

	Graphics graphicsManager;
	graphicsManager.Initialize("Game Title");
	systems.push_back(&graphicsManager);

	Physics physicsManager;
	systems.push_back(&physicsManager);

	InputManager inputManager;

	//Game Loop
	while (!glfwWindowShouldClose(graphicsManager.getWindow())) {
		//Calculate Delta Time
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//Iterate Through Each System and Call Their Update Methods
		for (vector<System*>::iterator it = systems.begin(); it != systems.end(); ++it) {
			(*it)->Update(deltaTime);
		}

		inputManager.Update();
	}
}
