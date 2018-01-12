#pragma once

#include <GLFW\glfw3.h>
#include <iostream>
using namespace std;

class Controller {
public:
	static bool IsControllerActive();

	static void Update();
private:
	static int axesCount;
	static const float *axes;
};