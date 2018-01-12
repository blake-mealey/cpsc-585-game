#include "Controller.h"

const float* Controller::axes;
int Controller::axesCount = -1;

const unsigned char* Controller::buttons;
int Controller::buttonsCount = -1;

int Controller::NumControllersActive() {
	int tmp = 0;
	for (int i = 0; i < GLFW_JOYSTICK_LAST; i++) {
		if (glfwJoystickPresent(i)) {
			tmp++;
		}
	}
	return tmp;
}

void Controller::Update() {
	for (int i = 0; i < GLFW_JOYSTICK_LAST; i++) {
		if (glfwJoystickPresent(i)) {

			axes = glfwGetJoystickAxes(i, &axesCount);				//Get Array of Axes and Number of Axes
			buttons = glfwGetJoystickButtons(i, &buttonsCount);		//Get Array of Buttons and Number of Buttons

			//Handles Input
			HandleAxes(i);
			HandleButtons(i);
		}
	};
}

void Controller::HandleAxes(int controllerNumber) {
	//Axes
	/*
	0: Left Stick X Axis
	1: Left Stick Y Axis
	2: Triggers? LOL
	3: Right Stick Y Axis
	4: Right Stick X Axis
	*/

	//Left Stick Axes
	//Left Stick X Axis
	if (axes[0] > 0.2 || axes[0] < -0.2) {
		cout << "Controller " << (controllerNumber + 1) << " Left Stick X Axis: " << axes[0] << endl;
	}
	//Left Stick Y Axis
	if (axes[1] > 0.2 || axes[1] < -0.2) {
		cout << "Controller " << (controllerNumber + 1) << " Left Stick Y Axis: " << -axes[1] << endl;
	}

	//Right Stick Axes
	//Right Stick X Axis
	if (axes[4] > 0.2 || axes[4] < -0.2) {
		cout << "Controller " << (controllerNumber + 1) << " Right Stick X Axis: " << axes[4] << endl;
	}
	//Right Stick Y Axis
	if (axes[3] > 0.2 || axes[3] < -0.2) {
		cout << "Controller " << (controllerNumber + 1) << " Right Stick Y Axis: " << -axes[3] << endl;
	}

	//Triggers
	//Left Trigger
	if (axes[2] > 0.1) {
		cout << "Controller " << (controllerNumber + 1) << " Left Trigger: " << axes[2] << endl;
	}
	//Right Trigger
	if (axes[2] < -0.1) {
		cout << "Controller " << (controllerNumber + 1) << " Right Trigger: " << axes[2] << endl;
	}
}

void Controller::HandleButtons(int controllerNumber) {
	//Buttons
	/*
	0: A Button
	1: B Button
	2: X Button
	3: Y Button
	4: Left Bumper
	5: Right Bumper
	6: Back Button
	7: Start Button
	8: Left Stick
	9: Right Stick
	10: D-Pad Up
	11: D-Pad Right
	12: D-Pad Down
	13: D-Pad Left
	*/
	if (buttons[14] == GLFW_PRESS) {
		cout << "Pressed" << endl;
	}
}