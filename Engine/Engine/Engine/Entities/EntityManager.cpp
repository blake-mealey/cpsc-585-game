#include "EntityManager.h"

std::vector<Entity*> EntityManager::staticEntities;
std::vector<Entity*> EntityManager::dynamicEntities;
std::map<size_t, Entity*> EntityManager::idToEntity;
std::map<std::string, std::vector<Entity*>> EntityManager::tagToEntities;

std::map<ComponentType, std::vector<Component*>> EntityManager::components;

size_t EntityManager::nextEntityId = 0;

Entity* EntityManager::FindEntity(size_t id) {
	return idToEntity[id];
}

std::vector<Entity*> EntityManager::FindEntities(std::string tag) {
	return tagToEntities[tag];
}

Entity* EntityManager::CreateDynamicEntity() {
	return CreateEntity(dynamicEntities);
}

Entity* EntityManager::CreateStaticEntity() {
	return CreateEntity(staticEntities);
}

Entity* EntityManager::CreateEntity(std::vector<Entity*> &entities) {
	size_t id = nextEntityId++;
	Entity* entity = new Entity(id);
	entities.push_back(entity);
	idToEntity[id] = entity;
	return entity;
}

void EntityManager::DestroyDynamicEntity(size_t id) {
	DestroyEntity(id, dynamicEntities);
}

void EntityManager::SetTag(size_t entityId, std::string tag) {
	return SetTag(FindEntity(entityId), tag);
}

void EntityManager::SetTag(Entity* entity, std::string tag) {
	auto it = tagToEntities.find(entity->GetTag());
	if (it != tagToEntities.end()) {
		std::vector<Entity*> entities = it->second;
		auto it2 = std::find(entities.begin(), entities.end(), entity);
		if (it2 != entities.end())
			entities.erase(it2);
	}
	entity->SetTag(tag);
	tagToEntities[tag].push_back(entity);
}

void EntityManager::DestroyStaticEntity(size_t id) {
	DestroyEntity(id, staticEntities);
}

void EntityManager::DestroyEntity(size_t id, std::vector<Entity*> &entities) {
	Entity* entity = FindEntity(id);
	if (entity != nullptr) {
		idToEntity.erase(id);
		auto it = std::find(entities.begin(), entities.end(), entity);
		entities.erase(it);
	}
}

void EntityManager::AddComponent(size_t entityId, Component* component) {
	AddComponent(FindEntity(entityId), component);
}

void EntityManager::AddComponent(Entity* entity, Component* component) {
	entity->AddComponent(component);
	components[component->GetType()].push_back(component);
	component->SetEntity(entity);
}

std::vector<Component*> EntityManager::GetComponents(ComponentType type) {
	return components[type];
}

void EntityManager::BroadcastEvent(Event* event) {
	for (size_t i = 0; i < staticEntities.size(); i++) {
		staticEntities[i]->HandleEvent(event);
	}
	for (size_t i = 0; i < dynamicEntities.size(); i++) {
		dynamicEntities[i]->HandleEvent(event);
	}
}
