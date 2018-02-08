#include "ColliderComponent.h"

ComponentType ColliderComponent::GetType() {
	return ComponentType_Collider;
}

void ColliderComponent::HandleEvent(Event *event) {}