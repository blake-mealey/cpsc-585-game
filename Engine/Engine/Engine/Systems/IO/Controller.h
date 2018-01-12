#pragma once

#include <GLFW\glfw3.h>
#include <iostream>
using namespace std;

class Controller {
public:
	static int NumControllersActive();
	static void Update();

	static void HandleAxes(int controllerNumber);
	static void HandleButtons(int controllerNumber);
private:
	static int axesCount;
	static const float* axes;

	static int buttonsCount;
	static const unsigned char* buttons;
};