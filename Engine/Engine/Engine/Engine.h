#pragma once

#include "GLFW\glfw3.h"
#pragma comment(lib, "opengl32.lib")

#include <iostream>
using namespace std;

class Engine {
public:
	//Variables for Window Dimensions
	static int SCREEN_WIDTH;
	static int SCREEN_HEIGHT;

	//Constructor
	Engine();
	//Deconstructor
	~Engine();

	//Initialization
	bool Initialize(char* windowTitle);

	//Game Loop Stuff
	void Update();
	void BeginRender();
	void EndRender();

private:
	//Window Variable
	static GLFWwindow* window;

};