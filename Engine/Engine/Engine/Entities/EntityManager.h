#pragma once

#include "Entity.h"
#include <map>

class EntityManager {
public:
	// Access entities
	static Entity* FindEntity(int id);
	static std::vector<Entity*> FindEntities(std::string tag);
	// TODO: Way to iterate through all entities from other systems (return an iterator?)

	// Manage entities
	static Entity* CreateEntity();
	static void DestroyEntity(int id);
	
	// Contact entities
	static void BroadcastEvent(Event *event);
private:
	// Store entities
	static std::vector<Entity*> staticEntities;
	static std::vector<Entity*> dynamicEntities;
	static std::map<int, Entity*> idToEntity;

	// TODO (if necessary): Object pools
};
