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

			/* this is to detect when new controllers are plugged in or removed
			if (numberOfControllers >= NumControllersActive()) {
				numberOfControllers = NumControllersActive();
				cout << "controller removed" << endl;
				//send events
			}

			if (numberOfControllers <= NumControllersActive()) {
				numberOfControllers = NumControllersActive();
				cout << "controller added" << endl;
			}
			*/

			//Handles Input
			HandleAxes(i);
			HandleButtons(i);
		}
	};
}

//TODO: send events
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
	if (buttons[0] == GLFW_PRESS) {
		cout << "Controller " << (controllerNumber + 1) << " Pressed A" << endl;
	}
	if (buttons[1] == GLFW_PRESS) {
		cout << "Controller " << (controllerNumber + 1) << " Pressed B" << endl;
	}
	if (buttons[2] == GLFW_PRESS) {
		cout << "Controller " << (controllerNumber + 1) << " Pressed X" << endl;
	}
	if (buttons[3] == GLFW_PRESS) {
		cout << "Controller " << (controllerNumber + 1) << " Pressed Y" << endl;
	}
	if (buttons[4] == GLFW_PRESS) {
		cout << "Controller " << (controllerNumber + 1) << " Pressed LB" << endl;
	}
	if (buttons[5] == GLFW_PRESS) {
		cout << "Controller " << (controllerNumber + 1) << " Pressed RB" << endl;
	}
	if (buttons[6] == GLFW_PRESS) {
		cout << "Controller " << (controllerNumber + 1) << " Pressed Back" << endl;
	}
	if (buttons[7] == GLFW_PRESS) {
		cout << "Controller " << (controllerNumber + 1) << " Pressed Start" << endl;
	}
	if (buttons[8] == GLFW_PRESS) {
		cout << "Controller " << (controllerNumber + 1) << " Pressed Left stick" << endl;
	}
	if (buttons[9] == GLFW_PRESS) {
		cout << "Controller " << (controllerNumber + 1) << " Pressed Right stick" << endl;
	}
	if (buttons[10] == GLFW_PRESS) {
		cout << "Controller " << (controllerNumber + 1) << " Pressed D-Up" << endl;
	}
	if (buttons[11] == GLFW_PRESS) {
		cout << "Controller " << (controllerNumber + 1) << " Pressed D-Right" << endl;
	}
	if (buttons[12] == GLFW_PRESS) {
		cout << "Controller " << (controllerNumber + 1) << " Pressed D-Down" << endl;
	}
	if (buttons[13] == GLFW_PRESS) {
		cout << "Controller " << (controllerNumber + 1) << " Pressed D-Left" << endl;
	}
}