#include "Mouse.h"

glm::vec3 Mouse::cursorPosition = { 0, 0, 0 };

bool Mouse::buttonsDown[GLFW_MOUSE_BUTTON_LAST] = { false };
bool Mouse::buttonsPressed[GLFW_MOUSE_BUTTON_LAST] = { false };
bool Mouse::buttonsReleased[GLFW_MOUSE_BUTTON_LAST] = { false };

void Mouse::MousePositionCallback(GLFWwindow* window, double _x, double _y) {
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	cursorPosition = { _x, height - _y, 0 };
}

void Mouse::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if (button < 0) {
		return;
	}

	if (action == GLFW_PRESS && buttonsDown[button] == false) {
		buttonsPressed[button] = true;
		buttonsReleased[button] = false;
	}

	if (action == GLFW_RELEASE && buttonsDown[button] == true) {
		buttonsDown[button] = false;
		buttonsReleased[button] = true;
	}

	buttonsDown[button] = action != GLFW_RELEASE;
}

glm::vec3 Mouse::GetCursorPosition() {
	return cursorPosition;
}

bool Mouse::ButtonDown(int button) {
	return buttonsDown[button];
}

bool Mouse::ButtonPressed(int button) {
	bool tmp = buttonsPressed[button];
	buttonsPressed[button] = false;
	return tmp;
}

bool Mouse::ButtonReleased(int button) {
	bool tmp = buttonsReleased[button];
	buttonsReleased[button] = false;
	return tmp;
}