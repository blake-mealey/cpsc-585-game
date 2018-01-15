#include "MeshComponent.h"
#include "../Systems/Content/ContentManager.h"

ComponentType MeshComponent::GetType() {
	return ComponentType_MeshComponent;
}

void MeshComponent::HandleEvent(Event* event) {}

MeshComponent::MeshComponent(const std::string meshPath, Material *_material, const std::string texturePath) {
	mesh = ContentManager::GetMesh(meshPath);
    material = _material;
    texture = ContentManager::GetTexture(texturePath);
	transform = Transform();
}

Mesh* MeshComponent::GetMesh() const {
	return mesh;
}

