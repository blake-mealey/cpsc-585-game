#pragma once

#include "Component.h"

class RigidbodyComponent : public Component {
public:
	ComponentType GetType();
	void HandleEvent(Event *event);
private:

};