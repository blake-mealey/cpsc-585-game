#pragma once

#include "Component.h"

class ColliderComponent : public Component {
public:
	ComponentType GetType();
	void HandleEvent(Event *event);
private:

};