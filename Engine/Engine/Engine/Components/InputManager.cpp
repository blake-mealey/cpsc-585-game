#include "InputManager.h"

InputManager::InputManager() {

}

void InputManager::HandleMouse() {
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
}

void InputManager::HandleKeyboard() {
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

void InputManager::Update() {
	//Check for a Controller - If Controller Then Ignore Keyboard/Mouse Inputs
	if (Controller::NumControllersActive() < 1) {
		//Mouse and Keyboard Inputs
		HandleMouse();
		HandleKeyboard();
	}
	else {
		//Controller Inputs
		Controller::Update();
	}
}
