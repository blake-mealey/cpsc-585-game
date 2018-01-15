#pragma once

#include "System.h"
#include "IO/Mouse.h"
#include "IO/Keyboard.h"
#include "IO/Controller.h"

#pragma comment(lib, "opengl32.lib")	// TODO: Do this in the project properties?
//#include <GL/glew.h>					// Only necessary on Windows
#include <GLFW/glfw3.h>

class Graphics : public System {
public:
	//Variables for Window Dimensions
	static int SCREEN_WIDTH;
	static int SCREEN_HEIGHT;

	bool Initialize(char* windowTitle);
	void Update(Time deltaTime) override;

	GLFWwindow* getWindow();

private:
	static GLFWwindow* window;
};
