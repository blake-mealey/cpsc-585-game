#include "Engine/Engine.h"

#include <GLFW/glfw3.h>
#pragma comment(lib, "opengl32.lib")

using namespace std;

int main() {
	cout << "Hello, World!" << endl;

	Engine engine;

	engine.Initialize("Game Title");

	//Game Loop
	while(true) {
	
		engine.Update();
		engine.Render();

	}
}
