#include "Engine/Engine.h"
#include "Engine/Components/InputManager.h"

#include <GLFW/glfw3.h>
#include "Engine/Systems/Graphics.h"
#include <vector>
#pragma comment(lib, "opengl32.lib")

using namespace std;

int main() {
	cout << "Hello, World!" << endl;

	vector<System*> systems;

	Graphics graphics;
	graphics.Initialize("Game Title");
	systems.push_back(&graphics);

	InputManager inputManager;

	//Game Loop
	while (true) {
		Time dt = Time();
		for (vector<System*>::iterator it = systems.begin(); it != systems.end(); ++it) {
			(*it)->Update(dt);
		}

		inputManager.Update();
	}
}
