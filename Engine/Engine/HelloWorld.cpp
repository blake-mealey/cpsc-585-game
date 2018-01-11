#include "Engine/Engine.h"
#include "Engine\Systems\IO\Mouse.h"

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

	//Game Loop
	while(true) {
		Time dt = Time();
		for(vector<System*>::iterator it = systems.begin(); it != systems.end(); ++it) {
			(*it)->Update(dt);
		}

		if (Mouse::ButtonPressed(GLFW_MOUSE_BUTTON_1)) {
			cout << "Left Button Pressed" << endl;
		}
		if (Mouse::ButtonDown(GLFW_MOUSE_BUTTON_1)) {
			cout << "Left Button Held" << endl;
		}
		if (Mouse::ButtonReleased(GLFW_MOUSE_BUTTON_1)) {
			cout << "Left Button Released" << endl;
		}
	}
}
