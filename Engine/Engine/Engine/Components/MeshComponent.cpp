#include "MeshComponent.h"
#include "../Systems/Content/ContentManager.h"
#include "../Entities/Entity.h"
// not to sure what to do with this
glm::vec3 ToCylinder(glm::vec3 point, float radius, float ratio);

ComponentType MeshComponent::GetType() {
	return ComponentType_Mesh;
}

void MeshComponent::HandleEvent(Event* event) {}

MeshComponent::MeshComponent(nlohmann::json data) : transform(Transform()) {
	mesh = ContentManager::GetMesh(data["Mesh"]);
	material = ContentManager::GetMaterial(data["Material"]);
	if (!data["Texture"].is_null()) texture = ContentManager::GetTexture(data["Texture"]);
	else texture = nullptr;
	if (!data["UvScale"].is_null()) uvScale = ContentManager::JsonToVec2(data["UvScale"]);
	else uvScale = glm::vec2(1);
	if (!data["Cylinder"].is_null() && data["Cylinder"]) MakeCylinder(mesh); //TODO: store the cylinder radius for later conversions
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
	for (int i = 1; i < mesh->vertexCount; ++i) {
		maxX = std::max(maxX, mesh->vertices[i].x);
		minX = std::min(minX, mesh->vertices[i].x);
	}
	float R = (maxX- minX) / 12.5663706144;
	for (int i = 0; i < mesh->vertexCount; ++i) {
		glm::vec3 point = { mesh->vertices[i].x, mesh->vertices[i].y, mesh->vertices[i].z };
		point = ToCylinder(point, R, 1.f);
		mesh->vertices[i] = point;
	}
	mesh->GenerateNormals();
	//return radius
	return R;
}

glm::vec3 ToCylinder(glm::vec3 point, float radius, float ratio) {
	float theta = point.x * 2 * ratio / 3.141592;
	float r = radius - point.y;
	float h = point.z;

	point.x = r * cos(theta);
	point.y = r * sin(theta);
	point.z = h;

	return point;
}