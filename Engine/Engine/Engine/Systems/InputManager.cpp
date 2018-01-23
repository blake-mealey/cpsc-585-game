#include "InputManager.h"

vector<XboxController*> InputManager::xboxControllers;

InputManager &InputManager::Instance() {
	for (int i = 0; i < XUSER_MAX_COUNT; i++) {
		xboxControllers.push_back(new XboxController(i + 1));
	}

	static InputManager instance;
	return instance;
}

void InputManager::Update(Time _deltaTime) {
	HandleMouse();
	HandleKeyboard();
	HandleController();
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

void InputManager::HandleController() {

	for (auto it = xboxControllers.begin(); it != xboxControllers.end(); it++) {
		if ((*it)->IsConnected()) {
			//If Left Trigger - Vibrate Left Side
			if ((*it)->GetState().Gamepad.bLeftTrigger > 5) {
				(*it)->Vibrate(65535, -1);
				cout << "Controller: " << (*it)->GetControllerNumber() << " Left Trigger Down." << endl;
			}
			else {
				(*it)->Vibrate();
			}
			//If Right Trigger - Vibrate Right Side
			if ((*it)->GetState().Gamepad.bRightTrigger > 5) {
				(*it)->Vibrate(-1, 65535);
				cout << "Controller: " << (*it)->GetControllerNumber() << " Right Trigger Down." << endl;
			}
			else {
				(*it)->Vibrate();
			}
		}
	}
}