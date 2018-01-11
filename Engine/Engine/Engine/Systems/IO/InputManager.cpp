#include "InputManager.h"

InputManager::InputManager() {

}

void InputManager::Update() {

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