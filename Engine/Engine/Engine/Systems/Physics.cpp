#include "Physics.h"

#include <iostream>
using namespace std;

// Singleton
Physics* Physics::singletonInstance = nullptr;
Physics::Physics() { }
Physics* Physics::Instance() {
	if (!singletonInstance) {
		singletonInstance = new Physics();
	}
	return singletonInstance;
}

void Physics::Update(Time deltaTime) { }