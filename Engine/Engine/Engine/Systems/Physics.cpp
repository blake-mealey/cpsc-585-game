#include "Physics.h"

#include <iostream>
using namespace std;

// Singleton
Physics::Physics() { }
Physics &Physics::Instance() {
	static Physics instance;
	return instance;
}

void Physics::Initialize() {
    
}


void Physics::Update(Time currentTime, Time deltaTime) { }