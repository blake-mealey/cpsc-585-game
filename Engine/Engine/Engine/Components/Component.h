#pragma once

class Event;

enum ComponentType {
	ComponentType_MeshComponent,
	ComponentType_CameraComponent
};

class Component {
public:
	bool enabled;
	
	virtual ComponentType GetType() = 0;
	virtual void HandleEvent(Event *event) = 0;
};
