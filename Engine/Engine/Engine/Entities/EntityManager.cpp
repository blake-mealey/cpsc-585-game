#include "EntityManager.h"

std::vector<Entity*> EntityManager::staticEntities;
std::vector<Entity*> EntityManager::dynamicEntities;
std::map<size_t, Entity*> EntityManager::idToEntity;

std::map<ComponentType, std::vector<Component*>> EntityManager::components;

size_t EntityManager::nextEntityId = 0;

Entity* EntityManager::FindEntity(size_t id) {
	return idToEntity[id];
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
