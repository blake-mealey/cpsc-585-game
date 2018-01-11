#pragma once

class Event;

enum ComponentType {
	
};

class Component {
public:
	static ComponentType type;
	bool enabled;

	void HandleEvent(Event *event);
};
