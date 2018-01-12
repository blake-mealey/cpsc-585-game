#include "Engine/Engine.h"
#include "Engine/Components/InputManager.h"

#include <GLFW/glfw3.h>
#include "Engine/Systems/Graphics.h"
#include <vector>
#pragma comment(lib, "opengl32.lib")

using namespace std;

int main() {
	cout << "Hello, World!" << endl;

	//Delta Time Variables
	Time currentFrame;
	Time lastFrame(0);
	Time deltaTime(0);

	vector<System*> systems;

	Graphics graphics;
	graphics.Initialize("Game Title");
	systems.push_back(&graphics);

	InputManager inputManager;

	//Game Loop
	while (!glfwWindowShouldClose(graphics.getWindow())) {
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
