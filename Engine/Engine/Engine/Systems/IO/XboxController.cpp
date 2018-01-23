#include "XboxController.h"

XboxController::XboxController(int _playerNumber) {
	controllerNumber = _playerNumber - 1;
}

XINPUT_STATE XboxController::GetState() {
	//Zeroise the state
	ZeroMemory(&controllerState, sizeof(XINPUT_STATE));

	//Get the state
	XInputGetState(controllerNumber, &controllerState);

	return controllerState;
}

bool XboxController::IsConnected() {
	//Zeroise the state
	ZeroMemory(&controllerState, sizeof(XINPUT_STATE));

	//Get the state
	DWORD result = XInputGetState(controllerNumber, &controllerState);

	if (result == ERROR_SUCCESS) {
		return true;
	}
	else {
		return false;
	}
}

int XboxController::GetControllerNumber() {
	return controllerNumber;
}

void XboxController::Vibrate(int _leftVal, int _rightVal) {
	//Create Vibration State
	XINPUT_VIBRATION vibration;

	//Zeroise the Vibration
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

	//Set Vibration Values from 0 to 65535
	if (_leftVal == -1) {
		vibration.wRightMotorSpeed = _rightVal;
	} else if (_rightVal == -1) {
		vibration.wLeftMotorSpeed = _leftVal;
	} else {
		vibration.wLeftMotorSpeed = _leftVal;
		vibration.wRightMotorSpeed = _rightVal;
	}

	//Vibrate the Controller
	XInputSetState(controllerNumber, &vibration);
}