#include "Engine/Engine.h"
#include "Engine\Systems\IO\Mouse.h"

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

		if (Mouse::ButtonPressed(GLFW_MOUSE_BUTTON_1)) {
			cout << "Left Button Pressed" << endl;
		}
		if (Mouse::ButtonDown(GLFW_MOUSE_BUTTON_1)) {
			cout << "Left Button Held" << endl;
		}
		if (Mouse::ButtonReleased(GLFW_MOUSE_BUTTON_1)) {
			cout << "Left Button Released" << endl;
		}

		engine.BeginRender();
		//Draw Stuff in Here
		engine.EndRender();
	}
}
