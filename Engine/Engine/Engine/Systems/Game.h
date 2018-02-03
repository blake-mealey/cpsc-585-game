#pragma once
#include "System.h"

class Entity;

class Game : public System {
public:
	// Access the singleton instance
	static Game& Instance();

	void Initialize();

	void Update(Time currentTime, Time deltaTime) override;

private:
	// No instantiation or copying
	Game();
	Game(const Game&) = delete;
	Game& operator= (const Game&) = delete;

	Entity *boulder;
	Entity *camera;
	Entity *sun;
	Entity *floor;
};
