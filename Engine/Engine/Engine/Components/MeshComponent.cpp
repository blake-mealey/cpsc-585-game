#include "MeshComponent.h"
#include "../Systems/Content/ContentManager.h"

ComponentType MeshComponent::GetType() {
	return ComponentType_MeshComponent;
}

void MeshComponent::HandleEvent(Event* event) {}

MeshComponent::MeshComponent(const std::string meshPath) {
	mesh = ContentManager::GetMesh(meshPath);
	transform = Transform();
}

Mesh* MeshComponent::GetMesh() const {
	return mesh;
}

