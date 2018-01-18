#pragma once
#include "../Engine/Systems/Graphics.h"

#include <iostream>
#include <string>
using namespace std;

class Controller {
public:
	static void Update();

	static int NumControllersConnected();
	static void DetectControllers();

	static void HandleAxes(int controllerNumber);
	static void HandleButtons(int controllerNumber);

	static bool AxesDown(int axis);
	static bool AxesPressed(int axis);
	static bool AxesReleased(int axis);

	static bool ButtonsDown(int button);
	static bool ButtonsPressed(int button);
	static bool ButtonsReleased(int button);

private:
	static const int NUMBER_AXES = 5;
	static const int NUMBER_BUTTONS = 14;
	static int numConnectedControllers;
	
	static const float* axes;
	static int axesCount;

	static const unsigned char* buttons;
	static int buttonsCount;

	static bool axesDown[];
	static bool axesPressed[];
	static bool axesReleased[];

	static bool buttonsDown[];
	static bool buttonsPressed[];
	static bool buttonsReleased[];
};