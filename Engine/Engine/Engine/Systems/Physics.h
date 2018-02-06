#pragma once
#include "System.h"

class Physics : public System {
public:
	// Access the singleton instance
	static Physics& Instance();

    void Initialize();

	void Update(Time currentTime, Time deltaTime) override;

private:
	// No instantiation or copying
	Physics();
	Physics(const Physics&) = delete;
	Physics& operator= (const Physics&) = delete;
};