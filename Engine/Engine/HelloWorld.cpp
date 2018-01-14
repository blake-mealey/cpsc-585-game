#include "Engine/Engine.h"
#include "Engine/Components/InputManager.h"

#include <GLFW/glfw3.h>
#include "Engine/Systems/Graphics.h"
#include <vector>
#pragma comment(lib, "opengl32.lib")

using namespace std;

int main() {
	cout << "Hello, World!" << endl;

	Time globalTime(0);
	Time lastFrame(0);
	Time deltaTime(0);

	vector<System*> systems;

	Graphics graphics;
	graphics.Initialize("Game Title");
	systems.push_back(&graphics);

	CameraComponent *camera = new CameraComponent();
	graphics.RegisterCamera(camera);

	MeshComponent *mesh = new MeshComponent("Cube");
	graphics.meshComponents.push_back(mesh);

	InputManager inputManager;
	
	float angle = 0;

	//Game Loop
	while (!glfwWindowShouldClose(graphics.GetWindow())) {
		//Calculate Delta Time
		Time currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		globalTime += deltaTime;

		mesh->transform.SetPosition(glm::vec3(0, sin(globalTime.GetTimeMilliSeconds()/500), 0));
		mesh->transform.Rotate(glm::vec3(0, 1, 1), 0.0005);

//		angle += 0.01;
//		camera->SetPosition(10.f * glm::vec3(sin(angle), 0, cos(angle)));

		//Iterate Through Each System and Call Their Update Methods
		for (vector<System*>::iterator it = systems.begin(); it != systems.end(); ++it) {
			(*it)->Update(deltaTime);	
		}

		inputManager.Update();
	}
}
