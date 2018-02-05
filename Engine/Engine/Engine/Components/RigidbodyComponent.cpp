#include "RigidbodyComponent.h"

ComponentType RigidbodyComponent::GetType() {
	return ComponentType_Rigidbody;
}

void RigidbodyComponent::HandleEvent(Event *event) {}