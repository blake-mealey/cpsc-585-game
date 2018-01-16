#include "InputManager.h"

InputManager::InputManager() {

}

/*
Check for current status of given buttons of the mouse, as well as cursor positions and handle their input
*/
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

/*
Check for current status of keyboard buttons and handle their input
*/
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
	if (Controller::NumControllersConnected() < 1) {
		//Mouse and Keyboard Inputs
		HandleMouse();
		HandleKeyboard();
	}
	else {
		//Controller Inputs
		Controller::Update();
	}
}
