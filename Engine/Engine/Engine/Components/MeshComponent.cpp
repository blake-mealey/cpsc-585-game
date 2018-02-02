#include "MeshComponent.h"
#include "../Systems/Content/ContentManager.h"
#include "../Entities/Entity.h"

#include <iostream>
// not to sure what to do with this
glm::vec3 ToCylinder(glm::vec3 point, float radius);
glm::vec3 FromCylinder(glm::vec3 point, float radius);

ComponentType MeshComponent::GetType() {
	return ComponentType_Mesh;
}

void MeshComponent::HandleEvent(Event* event) {}

MeshComponent::MeshComponent(nlohmann::json data) : transform(Transform()) {
	mesh = ContentManager::GetMesh(data["Mesh"]);
	material = ContentManager::GetMaterial(data["Material"]);
	if (!data["Texture"].is_null()) texture = ContentManager::GetTexture(data["Texture"]);
	else texture = nullptr;
	uvScale = ContentManager::JsonToVec2(data["UvScale"], glm::vec2(1.f));
    if (ContentManager::GetFromJson<bool>(data["Cylinder"], false)) MakeCylinder(mesh); //TODO: store the cylinder radius for later conversions
}

MeshComponent::MeshComponent(std::string meshPath, std::string materialPath) : texture(nullptr) {
	mesh = ContentManager::GetMesh(meshPath);
	material = ContentManager::GetMaterial(materialPath);
}

MeshComponent::MeshComponent(const std::string meshPath, const std::string materialPath, const std::string texturePath) : transform(Transform()) {
	mesh = ContentManager::GetMesh(meshPath);
    material = ContentManager::GetMaterial(materialPath);
    texture = ContentManager::GetTexture(texturePath);
}

MeshComponent::MeshComponent(std::string meshPath, Material *_material) : material(_material), texture(nullptr) {
	mesh = ContentManager::GetMesh(meshPath);
}

Mesh* MeshComponent::GetMesh() const {
	return mesh;
}

Material* MeshComponent::GetMaterial() const {
	return material;
}

Texture* MeshComponent::GetTexture() const {
	return texture;
}

glm::vec2 MeshComponent::GetUvScale() const {
	return uvScale;
}

void MeshComponent::SetEntity(Entity* _entity) {
	Component::SetEntity(_entity);
	transform.parent = &_entity->transform;
}

float MeshComponent::MakeCylinder(Mesh* mesh) {
	float maxX = mesh->vertices[0].x;
	float minX = mesh->vertices[0].x;
	for (size_t i = 1; i < mesh->vertexCount; ++i) {
		maxX = std::max(maxX, mesh->vertices[i].x);
		minX = std::min(minX, mesh->vertices[i].x);
	}

	float circumfrence = (maxX - minX);
	float R = circumfrence / 2.f / (float) M_PI;
	for (size_t i = 0; i < mesh->vertexCount; ++i) {
		glm::vec3 point = { mesh->vertices[i].x, mesh->vertices[i].y, mesh->vertices[i].z };
		point = FromCylinder(point,R);
		mesh->vertices[i] = point;
	}
	mesh->GenerateNormals();
	//return radius
	return R;
}

glm::vec3 ToCylinder(glm::vec3 point, float radius) {
	float theta = point.x / radius;
	float r = radius - point.y;

	point.x = r * cos(theta);
	point.y = r * sin(theta);

	return point;
}

glm::vec3 FromCylinder(glm::vec3 point, float radius) {
	float r = sqrt(point.x*point.x + point.y*point.y);
	float theta = atan2(point.y, point.x);

	point.x = theta * radius;
	point.y = radius - r;

	return point;
}