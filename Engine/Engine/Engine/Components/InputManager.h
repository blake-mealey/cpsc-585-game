#pragma once

#include "../Systems/IO/Keyboard.h"

using namespace std;

class InputManager {
public:
	InputManager();

	void HandleMouse();
	void HandleKeyboard();

	void Update();
private:

};