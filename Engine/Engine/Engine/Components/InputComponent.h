#pragma once

#include "../Systems/IO/Keyboard.h"

using namespace std;

class InputComponent {
public:
	InputComponent();

	void HandleMouse();
	void HandleKeyboard();

	void Update();
private:

};