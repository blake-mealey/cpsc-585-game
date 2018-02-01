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

void InputManager::Update(Time _deltaTime) {
	dt = _deltaTime;
	HandleMouse();
	HandleKeyboard();
	HandleController();
}

void InputManager::HandleMouse() {
	//Mouse Inputs

	//Left Mouse Button
	if (Mouse::ButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
		cout << "Left Mouse Button Pressed" << endl;
	} else if (Mouse::ButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
		cout << "Left Mouse Button Held" << endl;
	} else if (Mouse::ButtonReleased(GLFW_MOUSE_BUTTON_LEFT)) {
		cout << "Left Mouse Button Released" << endl;
	}

	//Right Mouse Button
	if (Mouse::ButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
		cout << "Right Mouse Button Pressed" << endl;
	} else if (Mouse::ButtonDown(GLFW_MOUSE_BUTTON_RIGHT)) {
		cout << "Right Mouse Button Held" << endl;
	} else if (Mouse::ButtonReleased(GLFW_MOUSE_BUTTON_RIGHT)) {
		cout << "Right Mouse Button Released" << endl;
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

			// -------------------------------------------------------------------------------------------------------------- //
			// TRIGGERS
			// -------------------------------------------------------------------------------------------------------------- //

			//Manage Trigger States
			//Left Trigger
			if ((*controller)->GetPreviousState().Gamepad.bLeftTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD && (*controller)->GetState().Gamepad.bLeftTrigger >= XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " pressed L-TRIGGER" << endl;
			} else if ((*controller)->GetPreviousState().Gamepad.bLeftTrigger >= XINPUT_GAMEPAD_TRIGGER_THRESHOLD && (*controller)->GetState().Gamepad.bLeftTrigger >= XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " held L-TRIGGER" << endl;
				leftVibrate = 30000 * (*controller)->GetState().Gamepad.bLeftTrigger / 255;
			} else if ((*controller)->GetPreviousState().Gamepad.bLeftTrigger >= XINPUT_GAMEPAD_TRIGGER_THRESHOLD && (*controller)->GetState().Gamepad.bLeftTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " released L-TRIGGER" << endl;
			}

			//Right Trigger
			if ((*controller)->GetPreviousState().Gamepad.bRightTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD && (*controller)->GetState().Gamepad.bRightTrigger >= XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " pressed R-TRIGGER" << endl;
			} else if ((*controller)->GetPreviousState().Gamepad.bRightTrigger >= XINPUT_GAMEPAD_TRIGGER_THRESHOLD && (*controller)->GetState().Gamepad.bRightTrigger >= XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " held R-TRIGGER" << endl;
				rightVibrate = 30000 * (*controller)->GetState().Gamepad.bRightTrigger / 255;
			} else if ((*controller)->GetPreviousState().Gamepad.bRightTrigger >= XINPUT_GAMEPAD_TRIGGER_THRESHOLD && (*controller)->GetState().Gamepad.bRightTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " released R-TRIGGER" << endl;
			}

			//Left Joystick X-Axis
			if (((*controller)->GetPreviousState().Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && (*controller)->GetPreviousState().Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) && (((*controller)->GetState().Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) || ((*controller)->GetState().Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " pressed LEFT-JOYSTICK X-AXIS" << endl;
			} else if ((((*controller)->GetPreviousState().Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) || ((*controller)->GetPreviousState().Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)) && (((*controller)->GetState().Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) || ((*controller)->GetState().Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " held LEFT-JOYSTICK X-AXIS" << endl;
			} else if (((*controller)->GetPreviousState().Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || (*controller)->GetPreviousState().Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) && (((*controller)->GetState().Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) && ((*controller)->GetState().Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " released LEFT-JOYSTICK X-AXIS" << endl;
			}

			//Left Joystick Y-Axis
			if (((*controller)->GetPreviousState().Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && (*controller)->GetPreviousState().Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) && (((*controller)->GetState().Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) || ((*controller)->GetState().Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " pressed LEFT-JOYSTICK Y-AXIS" << endl;
			} else if ((((*controller)->GetPreviousState().Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) || ((*controller)->GetPreviousState().Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)) && (((*controller)->GetState().Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) || ((*controller)->GetState().Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " held LEFT-JOYSTICK Y-AXIS" << endl;
			} else if (((*controller)->GetPreviousState().Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || (*controller)->GetPreviousState().Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) && (((*controller)->GetState().Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) && ((*controller)->GetState().Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " released LEFT-JOYSTICK Y-AXIS" << endl;
			}
			
			//Right Joystick X-Axis
			if (((*controller)->GetPreviousState().Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && (*controller)->GetPreviousState().Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) && (((*controller)->GetState().Gamepad.sThumbRX >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) || ((*controller)->GetState().Gamepad.sThumbRX <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " pressed RIGHT-JOYSTICK X-AXIS" << endl;
			} else if ((((*controller)->GetPreviousState().Gamepad.sThumbRX >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) || ((*controller)->GetPreviousState().Gamepad.sThumbRX <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)) && (((*controller)->GetState().Gamepad.sThumbRX >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) || ((*controller)->GetState().Gamepad.sThumbRX <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " held RIGHT-JOYSTICK X-AXIS" << endl;
			} else if (((*controller)->GetPreviousState().Gamepad.sThumbRX >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE || (*controller)->GetPreviousState().Gamepad.sThumbRX <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) && (((*controller)->GetState().Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) && ((*controller)->GetState().Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " released RIGHT-JOYSTICK X-AXIS" << endl;
			}

			//Right Joystick Y-Axis
			if (((*controller)->GetPreviousState().Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && (*controller)->GetPreviousState().Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) && (((*controller)->GetState().Gamepad.sThumbRY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) || ((*controller)->GetState().Gamepad.sThumbRY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " pressed RIGHT-JOYSTICK Y-AXIS" << endl;
			} else if ((((*controller)->GetPreviousState().Gamepad.sThumbRY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) || ((*controller)->GetPreviousState().Gamepad.sThumbRY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)) && (((*controller)->GetState().Gamepad.sThumbRY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) || ((*controller)->GetState().Gamepad.sThumbRY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " held RIGHT-JOYSTICK Y-AXIS" << endl;
			} else if (((*controller)->GetPreviousState().Gamepad.sThumbRY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE || (*controller)->GetPreviousState().Gamepad.sThumbRY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) && (((*controller)->GetState().Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) && ((*controller)->GetState().Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " released RIGHT-JOYSTICK Y-AXIS" << endl;
			}

			// -------------------------------------------------------------------------------------------------------------- //
			// BUTTONS
			// -------------------------------------------------------------------------------------------------------------- //

			//Manage Button States
			int heldButtons = (*controller)->GetState().Gamepad.wButtons & (*controller)->GetPreviousState().Gamepad.wButtons;
			int pressedButtons = ((*controller)->GetState().Gamepad.wButtons ^ (*controller)->GetPreviousState().Gamepad.wButtons) & (*controller)->GetState().Gamepad.wButtons;
			int releasedButtons = ((*controller)->GetState().Gamepad.wButtons ^ (*controller)->GetPreviousState().Gamepad.wButtons) & (*controller)->GetPreviousState().Gamepad.wButtons;

			//DPAD-UP
			if (pressedButtons & XINPUT_GAMEPAD_DPAD_UP) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " pressed DPAD-UP" << endl;
			} else if (heldButtons & XINPUT_GAMEPAD_DPAD_UP) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " held DPAD-UP" << endl;
			} else if (releasedButtons & XINPUT_GAMEPAD_DPAD_UP) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " released DPAD-UP" << endl;
			}

			//DPAD-DOWN
			if (pressedButtons & XINPUT_GAMEPAD_DPAD_DOWN) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " pressed DPAD-DOWN" << endl;
			} else if (heldButtons & XINPUT_GAMEPAD_DPAD_DOWN) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " held DPAD-DOWN" << endl;
			} else if (releasedButtons & XINPUT_GAMEPAD_DPAD_DOWN) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " released DPAD-DOWN" << endl;
			}

			//DPAD-LEFT
			if (pressedButtons & XINPUT_GAMEPAD_DPAD_LEFT) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " pressed DPAD-LEFT" << endl;
			} else if (heldButtons & XINPUT_GAMEPAD_DPAD_LEFT) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " held DPAD-LEFT" << endl;
			} else if (releasedButtons & XINPUT_GAMEPAD_DPAD_LEFT) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " released DPAD-LEFT" << endl;
			}

			//DPAD-RIGHT
			if (pressedButtons & XINPUT_GAMEPAD_DPAD_RIGHT) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " pressed DPAD-RIGHT" << endl;
			} else if (heldButtons & XINPUT_GAMEPAD_DPAD_RIGHT) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " held DPAD-RIGHT" << endl;
			} else if (releasedButtons & XINPUT_GAMEPAD_DPAD_RIGHT) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " released DPAD-RIGHT" << endl;
			}

			//START
			if (pressedButtons & XINPUT_GAMEPAD_START) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " pressed START" << endl;
			} else if (heldButtons & XINPUT_GAMEPAD_START) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " held START" << endl;
			} else if (releasedButtons & XINPUT_GAMEPAD_START) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " released START" << endl;
			}

			//BACK
			if (pressedButtons & XINPUT_GAMEPAD_BACK) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " pressed BACK" << endl;
			} else if (heldButtons & XINPUT_GAMEPAD_BACK) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " held BACK" << endl;
			} else if (releasedButtons & XINPUT_GAMEPAD_BACK) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " released BACK" << endl;
			}

			//LEFT-THUMB
			if (pressedButtons & XINPUT_GAMEPAD_LEFT_THUMB) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " pressed LEFT-THUMB" << endl;
			} else if (heldButtons & XINPUT_GAMEPAD_LEFT_THUMB) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " held LEFT-THUMB" << endl;
			} else if (releasedButtons & XINPUT_GAMEPAD_LEFT_THUMB) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " released LEFT-THUMB" << endl;
			}

			//RIGHT-THUMB
			if (pressedButtons & XINPUT_GAMEPAD_RIGHT_THUMB) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " pressed RIGHT-THUMB" << endl;
			} else if (heldButtons & XINPUT_GAMEPAD_RIGHT_THUMB) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " held RIGHT-THUMB" << endl;
			} else if (releasedButtons & XINPUT_GAMEPAD_RIGHT_THUMB) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " released RIGHT-THUMB" << endl;
			}

			//LEFT-SHOULDER
			if (pressedButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " pressed LEFT-SHOULDER" << endl;
			} else if (heldButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " held LEFT-SHOULDER" << endl;
			} else if (releasedButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " released LEFT-SHOULDER" << endl;
			}

			//RIGHT-SHOULDER
			if (pressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " pressed RIGHT-SHOULDER" << endl;
			} else if (heldButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " held RIGHT-SHOULDER" << endl;
			} else if (releasedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " released RIGHT-SHOULDER" << endl;
			}

			//A
			if (pressedButtons & XINPUT_GAMEPAD_A) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " pressed A" << endl;
			} else if (heldButtons & XINPUT_GAMEPAD_A) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " held A" << endl;
			} else if (releasedButtons & XINPUT_GAMEPAD_A) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " released A" << endl;
			}

			//B
			if (pressedButtons & XINPUT_GAMEPAD_B) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " pressed B" << endl;
			} else if (heldButtons & XINPUT_GAMEPAD_B) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " held B" << endl;
			} else if (releasedButtons & XINPUT_GAMEPAD_B) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " released B" << endl;
			}

			//X
			if (pressedButtons & XINPUT_GAMEPAD_X) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " pressed X" << endl;
			} else if (heldButtons & XINPUT_GAMEPAD_X) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " held X" << endl;
			} else if (releasedButtons & XINPUT_GAMEPAD_X) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " released X" << endl;
			}

			//Y
			if (pressedButtons & XINPUT_GAMEPAD_Y) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " pressed Y" << endl;
			} else if (heldButtons & XINPUT_GAMEPAD_Y) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " held Y" << endl;
			} else if (releasedButtons & XINPUT_GAMEPAD_Y) {
				cout << "Controller: " << (*controller)->GetControllerNumber() << " released Y" << endl;
			}

			//Vibrate Controller
			(*controller)->Vibrate(leftVibrate, rightVibrate);

			//Update Previous Controller State
			(*controller)->SetPreviousState((*controller)->GetState());
		}
	}
}