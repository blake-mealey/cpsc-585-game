#pragma once
#include "../Systems/IO/Controller.h"
#include "../Systems/IO/Keyboard.h"
#include "../Systems/IO/Mouse.h"

using namespace std;

class InputManager {
public:
	InputManager();

	void HandleMouse();
	void HandleKeyboard();

	void Update();
private:

};