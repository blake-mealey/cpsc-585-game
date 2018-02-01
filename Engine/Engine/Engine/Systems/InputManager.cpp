#include "InputManager.h"

#include "../Entities/EntityManager.h"

vector<XboxController*> InputManager::xboxControllers;

Time dt;

InputManager &InputManager::Instance() {
	for (int i = 0; i < XUSER_MAX_COUNT; i++) {
		xboxControllers.push_back(new XboxController(i + 1));
	}

	static InputManager instance;
	return instance;
}

void InputManager::Update(Time currentTime, Time _deltaTime) {
	dt = _deltaTime;
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

	//Cursor Position
	//cout << "Mouse Position: " << Mouse::GetCursorPosition().x << ", " << Mouse::GetCursorPosition().y << ", " << Mouse::GetCursorPosition().z << endl;
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
			int leftVibrate = 0;
			int rightVibrate = 0;

			//Manage Trigger States
			//Left Trigger
			if ((*controller)->GetPreviousState().Gamepad.bLeftTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD && (*controller)->GetState().Gamepad.bLeftTrigger >= XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " pressed L Trigger" << endl;
			} else if ((*controller)->GetPreviousState().Gamepad.bLeftTrigger >= XINPUT_GAMEPAD_TRIGGER_THRESHOLD && (*controller)->GetState().Gamepad.bLeftTrigger >= XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " held L Trigger" << endl;
				leftVibrate = 30000 * (*controller)->GetState().Gamepad.bLeftTrigger / 255;
			} else if ((*controller)->GetPreviousState().Gamepad.bLeftTrigger >= XINPUT_GAMEPAD_TRIGGER_THRESHOLD && (*controller)->GetState().Gamepad.bLeftTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " released L Trigger" << endl;
			}

			//Right Trigger
			if ((*controller)->GetPreviousState().Gamepad.bRightTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD && (*controller)->GetState().Gamepad.bRightTrigger >= XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " pressed R Trigger" << endl;
			} else if ((*controller)->GetPreviousState().Gamepad.bRightTrigger >= XINPUT_GAMEPAD_TRIGGER_THRESHOLD && (*controller)->GetState().Gamepad.bRightTrigger >= XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " held R Trigger" << endl;
				rightVibrate = 30000 * (*controller)->GetState().Gamepad.bRightTrigger / 255;
			} else if ((*controller)->GetPreviousState().Gamepad.bRightTrigger >= XINPUT_GAMEPAD_TRIGGER_THRESHOLD && (*controller)->GetState().Gamepad.bRightTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " released R Trigger" << endl;
			}

			//Left Joystick X
			if ((((*controller)->GetPreviousState().Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) || ((*controller)->GetPreviousState().Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)) && (((*controller)->GetState().Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) || ((*controller)->GetState().Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))){
				cout << (*controller)->GetState().Gamepad.sThumbLX << endl;
				EntityManager::FindEntities("Boulder")[(*controller)->GetControllerNumber()]->transform.Translate(glm::vec3(((*controller)->GetState().Gamepad.sThumbLX / 10000) * dt.GetTimeSeconds(), 0, 0));
			}

			//Left Joystick Y
			if ((((*controller)->GetPreviousState().Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) || ((*controller)->GetPreviousState().Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)) && (((*controller)->GetState().Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) || ((*controller)->GetState().Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))) {
				cout << (*controller)->GetState().Gamepad.sThumbLY << endl;
				EntityManager::FindEntities("Boulder")[(*controller)->GetControllerNumber()]->transform.Translate(glm::vec3(0.0f, 0, -((*controller)->GetState().Gamepad.sThumbLY / 10000) * dt.GetTimeSeconds()));
			}
			
			//Right Joystick X
			if ((((*controller)->GetPreviousState().Gamepad.sThumbRX >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) || ((*controller)->GetPreviousState().Gamepad.sThumbRX <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)) && (((*controller)->GetState().Gamepad.sThumbRX >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) || ((*controller)->GetState().Gamepad.sThumbRX <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))) {
				cout << (*controller)->GetState().Gamepad.sThumbRX << endl;
				EntityManager::FindEntities("Boulder")[(*controller)->GetControllerNumber()]->transform.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), ((float)(*controller)->GetState().Gamepad.sThumbRX / 327670.f) * dt.GetTimeSeconds());
			}

			//Right Joystick Y
			if ((((*controller)->GetPreviousState().Gamepad.sThumbRY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) || ((*controller)->GetPreviousState().Gamepad.sThumbRY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)) && (((*controller)->GetState().Gamepad.sThumbRY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) || ((*controller)->GetState().Gamepad.sThumbRY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))) {
				cout << (*controller)->GetState().Gamepad.sThumbRY << endl;
				EntityManager::FindEntities("Boulder")[(*controller)->GetControllerNumber()]->transform.Rotate(glm::vec3(1.0f, 0.0f, 0.0f), ((float)(*controller)->GetState().Gamepad.sThumbRY / 327670.f) * dt.GetTimeSeconds());
			}

			//Manage Button States
			int heldButtons = (*controller)->GetState().Gamepad.wButtons & (*controller)->GetPreviousState().Gamepad.wButtons;
			int pressedButtons = ((*controller)->GetState().Gamepad.wButtons ^ (*controller)->GetPreviousState().Gamepad.wButtons) & (*controller)->GetState().Gamepad.wButtons;
			int releasedButtons = ((*controller)->GetState().Gamepad.wButtons ^ (*controller)->GetPreviousState().Gamepad.wButtons) & (*controller)->GetPreviousState().Gamepad.wButtons;

			//A Button
			if (pressedButtons & XINPUT_GAMEPAD_A) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " pressed A" << endl;
			} else if (heldButtons & XINPUT_GAMEPAD_A) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " held A" << endl;
			} else if (releasedButtons & XINPUT_GAMEPAD_A) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " released A" << endl;
			}

			//Vibrate Controller
			(*controller)->Vibrate(leftVibrate, rightVibrate);

			//Update Previous Controller State
			(*controller)->SetPreviousState((*controller)->GetState());
		}
	}
}