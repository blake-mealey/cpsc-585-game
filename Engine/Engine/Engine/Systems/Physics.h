#pragma once
#include "System.h"

class Physics : public System {
public:
	// Access the singleton instance
	static Physics *Instance();

	void Update(Time deltaTime) override;

private:
	// Singleton instance and private constructor
	static Physics *singletonInstance;
	Physics();
};