#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Keyboard {
public:
	static void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	static bool KeyDown(int key);
	static bool KeyPressed(int key);
	static bool KeyReleased(int key);

private:
	static bool keyDown[];
	static bool keyPressed[];
	static bool keyReleased[];

};