#include "Component.h"

Component::Component() : entity(nullptr) { }


void Component::SetEntity(Entity* _entity) {
	entity = _entity;
}

Entity* Component::GetEntity() const {
	return entity;
}
