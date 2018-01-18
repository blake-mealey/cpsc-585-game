#include "MeshComponent.h"
#include "../Systems/Content/ContentManager.h"
#include "../Entities/Entity.h"

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

void MeshComponent::SetEntity(Entity* _entity) {
	Component::SetEntity(_entity);
	transform.parent = &_entity->transform;
}

