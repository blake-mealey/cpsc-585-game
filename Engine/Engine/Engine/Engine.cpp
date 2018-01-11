#include "Engine.h"
#include "Systems\IO\Mouse.h"

//Screen Dimensions Values
int Engine::SCREEN_WIDTH = 1024;
int Engine::SCREEN_HEIGHT = 768;

//GLFW Window
GLFWwindow* Engine::window = NULL;

Engine::Engine() {

}

Engine::~Engine() {

}

bool Engine::Initialize(char* windowTitle) {
	//Initialize GLFW
	if (!glfwInit()) {
		cout << "Error Initializing GLFW" << endl;
		return false;
	}

	//Create Window
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, windowTitle, NULL, NULL);
	if (window == NULL) {
		cout << "Error Creating Window" << endl;
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

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	int xPos = (mode->width - SCREEN_WIDTH) / 2;
	int yPos = (mode->height - SCREEN_HEIGHT) / 2;
	glfwSetWindowPos(window, xPos, yPos);

	//GL Setup
	//Viewport
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION_MATRIX);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -10, 10);
	glFrustum(-1, 1, -1, 1, 0, 100);
	glDepthRange(-10, 10);
	glMatrixMode(GL_MODELVIEW_MATRIX);

	//Alpha Blending
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return true;
}

void Engine::Update() {
	glfwPollEvents();
}

void Engine::BeginRender() {
	//Clear Back Buffer Before We Draw
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Engine::EndRender() {
	//Swap Buffers to Display New Frame
	glfwSwapBuffers(window);
}