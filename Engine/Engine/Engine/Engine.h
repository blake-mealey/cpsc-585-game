#pragma once

#pragma comment(lib, "opengl32.lib")	// TODO: Do this in the project properties?

#include <GL/glew.h>					// Only necessary on Windows
#include <GLFW/glfw3.h>

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