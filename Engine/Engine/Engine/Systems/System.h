#pragma once
#include "Time.h"

class System {
public:
	virtual void Update(Time deltaTime) = 0;

private:
};
