#pragma once

#include <string>
#include "Transform.h"
#include "../Components/Component.h"
#include <vector>

class EntityManager;
class Event;

class Entity {
friend class EntityManager;
public:
	Transform transform;

	void HandleEvent(Event *event);

	size_t GetId() const;
	std::string GetTag() const;
	bool MatchesTag(std::string _tag) const;

	void SetTag(std::string _tag);
private:
	Entity(size_t _id);
	void AddComponent(Component *component);

	size_t id;
	std::string tag;
	std::vector<Component*> components;
};
