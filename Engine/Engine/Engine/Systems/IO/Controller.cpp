#include "Controller.h"

int Controller::numConnectedControllers = 0;

const float* Controller::axes;
int Controller::axesCount = -1;

const unsigned char* Controller::buttons;
int Controller::buttonsCount = -1;

bool Controller::axesDown[NUMBER_AXES] = { false };
bool Controller::axesPressed[NUMBER_AXES] = { false };
bool Controller::axesReleased[NUMBER_AXES] = { false };

bool Controller::buttonsDown[NUMBER_BUTTONS] = { false };
bool Controller::buttonsPressed[NUMBER_BUTTONS] = { false };
bool Controller::buttonsReleased[NUMBER_BUTTONS] = { false };

/*
Array to map axis numbers to axis names
*/
string axisLookup[] = {
	"Left Stick X Axis",
	"Left Stick Y Axis",
	"Triggers",
	"Right Stick Y Axis",
	"Right Stick X Axis"
};

/*
Array to map button numbers to button names
*/
string buttonLookup[] = {
	"A",
	"B",
	"X",
	"Y",
	"Left Bumper",
	"Right Bumper",
	"Back Button",
	"Start Button",
	"Left Stick",
	"Right Stick",
	"D-Pad Up",
	"D-Pad Right",
	"D-Pad Down",
	"D-Pad Left"
};

/*
Update loop to check for controller inputs (there are no callbacks for controllers)
*/
void Controller::Update() {
	for (int i = GLFW_JOYSTICK_1; i < GLFW_JOYSTICK_LAST; i++) {
		if (glfwJoystickPresent(i)) {

			axes = glfwGetJoystickAxes(i, &axesCount);				//Get Array of Axes and Number of Axes
			buttons = glfwGetJoystickButtons(i, &buttonsCount);		//Get Array of Buttons and Number of Buttons

			DetectControllers();		//Detects Controller Connection and Disconnection

			HandleAxes(i);		//Handles Axis Inputs
			HandleButtons(i);	//Handles Button Inputs
		}
	};
}

/*
Return the number of controllers currently connected
*/
int Controller::NumControllersConnected() {
	int tmp = 0;
	for (int i = 0; i < GLFW_JOYSTICK_LAST; i++) {
		if (glfwJoystickPresent(i)) {
			tmp++;
		}
	}
	return tmp;
}

/*
Detects when controllers are connected or disconnected
*/
void Controller::DetectControllers() {
	if (numConnectedControllers > NumControllersConnected()) {
		numConnectedControllers = NumControllersConnected();
		cout << "Controller Disconnected, " << numConnectedControllers << " Controllers Connected" << endl;
	}

	if (numConnectedControllers < NumControllersConnected()) {
		numConnectedControllers = NumControllersConnected();
		cout << "Controller Connected, " << numConnectedControllers << " Controllers Connected" << endl;
	}
}

/*
Handles the input of a specific controllers axes
*/
void Controller::HandleAxes(int controllerNumber) {
	//Axes
	/*
	0: Left Stick X Axis
	1: Left Stick Y Axis
	2: Triggers? LOL
	3: Right Stick Y Axis
	4: Right Stick X Axis
	*/

	for (int axis = 0; axis < NUMBER_AXES; axis++) {
		if ((axes[axis] > 0.2 || axes[axis] < -0.2) && axesDown[axis] == false) {
			axesPressed[axis] = true;
			axesReleased[axis] = false;
		} else if (!(axes[axis] > 0.2 || axes[axis] < -0.2) && axesDown[axis] == true) {
			axesPressed[axis] = false;
			axesReleased[axis] = true;
		}
		axesDown[axis] = ((axes[axis] > 0.2 || axes[axis] < -0.2));

		if (AxesPressed(axis)) {
			cout << "Controller " << (controllerNumber + 1) << " Pressed " << axisLookup[axis] << ": " << axes[axis] << endl;
		}
		if (AxesDown(axis)) {
			cout << "Controller " << (controllerNumber + 1) << " Held " << axisLookup[axis] << ": " << axes[axis] << endl;
		}
		if (AxesReleased(axis)) {
			cout << "Controller " << (controllerNumber + 1) << " Released " << axisLookup[axis] << ": " << axes[axis] << endl;
		}
	}
}

/*
Handles the input of a specific controllers buttons
*/
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
	for (int button = 0; button < NUMBER_BUTTONS; button++) {
		if (buttons[button] == GLFW_PRESS && buttonsDown[button] == false) {
			buttonsPressed[button] = true;
			buttonsReleased[button] = false;
		}
		if (buttons[button] == GLFW_RELEASE && buttonsDown[button] == true) {
			buttonsPressed[button] = false;
			buttonsReleased[button] = true;
		}
		buttonsDown[button] = buttons[button] != GLFW_RELEASE;

		if (ButtonsPressed(button)) {
			cout << "Controller " << (controllerNumber + 1) << " Pressed " << buttonLookup[button] << endl;
		}
		if (ButtonsDown(button)) {
			cout << "Controller " << (controllerNumber + 1) << " Held " << buttonLookup[button] << endl;
		}
		if (ButtonsReleased(button)) {
			cout << "Controller " << (controllerNumber + 1) << " Released " << buttonLookup[button] << endl;
		}
	}
}

/*
Check whether a specific axis is held down
*/
bool Controller::AxesDown(int axis) {
	return axesDown[axis];
}

/*
Check whether a specific axis was pressed in the most recent frame
*/
bool Controller::AxesPressed(int axis) {
	bool tmp = axesPressed[axis];
	axesPressed[axis] = false;
	return tmp;
}

/*
Check whether a specific axis was released in the most recent frame
*/
bool Controller::AxesReleased(int axis) {
	bool tmp = axesReleased[axis];
	axesReleased[axis] = false;
	return tmp;
}

/*
Check whether a specific button is currently held down
*/
bool Controller::ButtonsDown(int button) {
	return buttonsDown[button];
}

/*
Check whether a specific button was pressed in the most recent frame
*/
bool Controller::ButtonsPressed(int button) {
	bool tmp = buttonsPressed[button];
	buttonsPressed[button] = false;
	return tmp;
}

/*
Check whether a specific button was released in the most recent frame
*/
bool Controller::ButtonsReleased(int button) {
	bool tmp = buttonsReleased[button];
	buttonsReleased[button] = false;
	return tmp;
}