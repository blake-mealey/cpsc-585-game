#pragma once

#include <GL/glew.h>					// Only necessary on Windows
#pragma comment(lib, "opengl32.lib")	// TODO: Do this in the project properties?
#include <GLFW/glfw3.h>
#include <string>

#include "System.h"
#include <vector>

#include "glm/glm.hpp"

#include "Content/Mesh.h"
#include "../Components/CameraComponent.h"
#include "../Components/MeshComponent.h"
#include "Time.h"

struct VAOs {
	enum { Vertices=0, Count };
};

struct VBOs {
	enum { Vertices=0, UVs, Normals, Count };
};

struct Shaders {
	enum { Program=0, Count };
};

class Graphics : public System {
public:
	static const std::string VERTEX_SHADER_FILE_NAME;
	static const std::string FRAGMENT_SHADER_FILE_NAME;
	
	static const size_t MAX_CAMERAS;

	//Variables for Window Dimensions
	static int SCREEN_WIDTH;
	static int SCREEN_HEIGHT;

	bool Initialize(char* windowTitle);
	void Update(Time deltaTime) override;

	GLFWwindow* GetWindow() const;

	bool RegisterCamera(CameraComponent *camera);
	void UnregisterCamera(CameraComponent *camera);

	std::vector<MeshComponent*> meshComponents;		// Temporary
private:
	static GLFWwindow* window;
	GLsizei windowWidth;
	GLsizei windowHeight;
	
	GLuint vboIds[VBOs::Count];		// Points and UVs
	GLuint vaoIds[VAOs::Count];
	GLuint shaderIds[Shaders::Count];

	std::vector<CameraComponent*> cameras;

    void LoadTexture(GLuint textureId, const char *uniformName);
    void LoadTexture(Texture *texture, std::string uniformName);

	void LoadBuffer(const glm::vec3 *vertices, const glm::vec2 *uvs, const glm::vec3 *normals, const size_t vertexCount);
	void LoadBuffer(Mesh *mesh);

	void DestroyIds();
	void GenerateIds();

	void InitializeVao();
	GLuint LoadShaderProgram();
};
