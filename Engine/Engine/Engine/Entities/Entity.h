#pragma once
#include <string>
#include "Transform.h"
#include "../Components/Component.h"
#include <vector>

class Event;

class Entity {
public:
	Transform transform;

	Component GetComponent(ComponentType type);
	void HandleEvent(Event *event);

	int GetId();
	std::string GetTag();
	bool MatchesTag(std::string tag);
private:
	int id;
	std::string tag;
	std::vector<Component> components;
};
