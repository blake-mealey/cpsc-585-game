#include "MeshComponent.h"
#include "../Systems/Content/ContentManager.h"
#include "../Entities/Entity.h"

ComponentType MeshComponent::GetType() {
	return ComponentType_Mesh;
}

void MeshComponent::HandleEvent(Event* event) {}

MeshComponent::MeshComponent(nlohmann::json data) : transform(Transform()) {
	mesh = ContentManager::GetMesh(data["Mesh"]);
	material = ContentManager::GetMaterial(data["Material"]);
	texture = ContentManager::GetTexture(data["Texture"]);
}

MeshComponent::MeshComponent(const std::string meshPath, const std::string materialPath, const std::string texturePath) : transform(Transform()) {
	mesh = ContentManager::GetMesh(meshPath);
    material = ContentManager::GetMaterial(materialPath);
    texture = ContentManager::GetTexture(texturePath);
}

Mesh* MeshComponent::GetMesh() const {
	return mesh;
}

void MeshComponent::SetEntity(Entity* _entity) {
	Component::SetEntity(_entity);
	transform.parent = &_entity->transform;
}

