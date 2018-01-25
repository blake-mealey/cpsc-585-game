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
	//Iterate through each controller
	for (auto controller = xboxControllers.begin(); controller != xboxControllers.end(); controller++) {
		//If controller is connected
		if ((*controller)->IsConnected()) {
			//Manage Trigger States
			if ((*controller)->GetPreviousState().Gamepad.bLeftTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD && (*controller)->GetState().Gamepad.bLeftTrigger >= XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " pressed L Trigger" << endl;
			} else if ((*controller)->GetPreviousState().Gamepad.bLeftTrigger >= XINPUT_GAMEPAD_TRIGGER_THRESHOLD && (*controller)->GetState().Gamepad.bLeftTrigger >= XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " held L Trigger" << endl;
			} else if ((*controller)->GetPreviousState().Gamepad.bLeftTrigger >= XINPUT_GAMEPAD_TRIGGER_THRESHOLD && (*controller)->GetState().Gamepad.bLeftTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " released L Trigger" << endl;
			}

			//Manage Button States
			int heldButtons = (*controller)->GetState().Gamepad.wButtons & (*controller)->GetPreviousState().Gamepad.wButtons;
			int pressedButtons = ((*controller)->GetState().Gamepad.wButtons ^ (*controller)->GetPreviousState().Gamepad.wButtons) & (*controller)->GetState().Gamepad.wButtons;
			int releasedButtons = ((*controller)->GetState().Gamepad.wButtons ^ (*controller)->GetPreviousState().Gamepad.wButtons) & (*controller)->GetPreviousState().Gamepad.wButtons;

			if (pressedButtons & XINPUT_GAMEPAD_A) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " pressed A" << endl;
			} else if (heldButtons & XINPUT_GAMEPAD_A) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " held A" << endl;
			} else if (releasedButtons & XINPUT_GAMEPAD_A) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " released A" << endl;
			}

			//Update Previous Controller State
			(*controller)->SetPreviousState((*controller)->GetState());
		}
	}
}