#pragma once
#include "Entity.h"
#include <map>

class EntityManager {
public:
	static Entity* FindEntity(int id);
	static std::vector<Entity*> FindEntities(std::string tag);

	static Entity* CreateEntity();
	static void DestroyEntity(int id);
	
	static void BroadcastEvent(Event *event);

private:
	static std::vector<Entity> staticEntities;
	static std::vector<Entity> dynamicEntities;
	static std::map<int, Entity> idToEntity;
};
