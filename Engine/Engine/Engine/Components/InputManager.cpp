#include "InputManager.h"

InputManager::InputManager() {

}

void InputManager::Update() {
	//Check for a Controller - If Controller Then Ignore Keyboard/Mouse Inputs
	if (!Controller::IsControllerActive()) {
		//Mouse Inputs
		if (Mouse::ButtonPressed(GLFW_MOUSE_BUTTON_1)) {
			cout << "Left Button Pressed" << endl;
		}
		if (Mouse::ButtonDown(GLFW_MOUSE_BUTTON_1)) {
			cout << "Left Button Held" << endl;
		}
		if (Mouse::ButtonReleased(GLFW_MOUSE_BUTTON_1)) {
			cout << "Left Button Released" << endl;
		}

		//Keyboard Inputs
		if (Keyboard::KeyPressed(GLFW_KEY_W)) {
			cout << "W Key Pressed" << endl;
		}
		if (Keyboard::KeyDown(GLFW_KEY_W)) {
			cout << "W Key Held" << endl;
		}
		if (Keyboard::KeyReleased(GLFW_KEY_W)) {
			cout << "W Key Released" << endl;
		}
	}
	else {
		//Controller Inputs
		Controller::Update();
	}
}