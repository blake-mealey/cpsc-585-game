#pragma once

class Time;

class System {
public:
	virtual void Update(Time deltaTime) = 0;
};
