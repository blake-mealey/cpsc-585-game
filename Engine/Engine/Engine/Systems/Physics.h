#pragma once
#include "System.h"

class Physics : public System {
public:
	// Access the singleton instance
	static Physics& Instance();

	void Update(Time deltaTime) override;

private:
	// No instantiation or copying
	Physics();
	Physics(const Physics&) = delete;
	Physics& operator= (const Physics&) = delete;
};