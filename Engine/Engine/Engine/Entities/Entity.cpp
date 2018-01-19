#include "Entity.h"
#include "../Events/Event.h"

Entity::Entity(size_t _id) : id(_id) {}

void Entity::HandleEvent(Event* event) {
	for (size_t i = 0; i < components.size(); i++) {
		components[i]->HandleEvent(event);		// TODO: Not really correct, use Colton's code
	}
}

void Entity::AddComponent(Component* component) {
	components.push_back(component);
}

size_t Entity::GetId() const {
	return id;
}

std::string Entity::GetTag() const {
	return tag;
}

bool Entity::HasTag(std::string _tag) const {
	return tag.compare(_tag) == 0;
}

void Entity::SetTag(std::string _tag) {
	tag = _tag;
}


