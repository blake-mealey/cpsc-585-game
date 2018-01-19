#pragma once

class Event;
class Entity;

enum ComponentType {
	ComponentType_Mesh,
	ComponentType_Camera
};

class Component {
public:
	Component();
	
	bool enabled;
	
	virtual ComponentType GetType() = 0;
	virtual void HandleEvent(Event *event) = 0;

	virtual void SetEntity(Entity *_entity);
protected:
	Entity *entity;
};
