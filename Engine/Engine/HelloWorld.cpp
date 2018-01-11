#include "Engine/Engine.h"
#include "Engine/Systems/IO/InputManager.h"

#include <GLFW/glfw3.h>
#pragma comment(lib, "opengl32.lib")

using namespace std;

int main() {
	cout << "Hello, World!" << endl;

	Engine engine;

	InputManager inputManager;

	engine.Initialize("Game Title");

	//Game Loop
	while(true) {
		engine.Update();
		inputManager.Update();

		engine.BeginRender();
		//Draw Stuff in Here
		engine.EndRender();
	}
}
