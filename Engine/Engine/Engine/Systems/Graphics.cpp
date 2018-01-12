#include "Graphics.h"
#include "IO/Mouse.h"
#include "IO/Keyboard.h"
#include "IO/Controller.h"

#include <iostream>

//Screen Dimensions Values
int Graphics::SCREEN_WIDTH = 1024;
int Graphics::SCREEN_HEIGHT = 768;

//GLFW Window
GLFWwindow* Graphics::window = NULL;
GLuint vertexbuffer;

bool Graphics::Initialize(char* windowTitle) {
	if (!glfwInit()) {
		std::cout << "Error Initializing GLFW" << std::endl;
		return false;
	}

	//Create Window
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, windowTitle, NULL, NULL);
	if (window == NULL) {
		std::cout << "Error Creating Window" << std::endl;
		return false;
	}

	//GLFW Setup
	glfwMakeContextCurrent(window);
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glfwSwapInterval(1);	//Swap Buffer Every Frame (Double Buffering)

							//Input Callbacks
	glfwSetCursorPosCallback(window, Mouse::MousePositionCallback);
	glfwSetMouseButtonCallback(window, Mouse::MouseButtonCallback);
	glfwSetKeyCallback(window, Keyboard::KeyboardCallback);
	//glfwSetJoystickCallback(Controller::ControllerCallback);

	// Centers window
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	int xPos = (mode->width - SCREEN_WIDTH) / 2;
	int yPos = (mode->height - SCREEN_HEIGHT) / 2;
	glfwSetWindowPos(window, xPos, yPos);

	//GL Setup
	//Viewport
	glViewport(0, 0, width, height);
	/*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -10, 10);
	glDepthRange(-10, 10);
	glMatrixMode(GL_MODELVIEW);*/
	//glFrustum(-1, 1, -1, 1, 0, 100);

	//Alpha Blending
	/*glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/

	return true;
}

void Graphics::Update(Time deltaTime) {
	glfwPollEvents();

	//Clear Back Buffer Before We Draw
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// TODO: Render

	//Swap Buffers to Display New Frame
	glfwSwapBuffers(window);
}
