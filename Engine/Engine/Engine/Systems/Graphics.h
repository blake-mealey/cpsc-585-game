#pragma once

#include <GL/glew.h>					// Only necessary on Windows
#pragma comment(lib, "opengl32.lib")	// TODO: Do this in the project properties?
#include <GLFW/glfw3.h>
#include <string>

#include "System.h"
#include <vector>

#include "glm/glm.hpp"

#include "Content/Mesh.h"
#include "Content/Texture.h"
#include "../Components/Component.h"
#include "Time.h"
#include "Content/ShaderProgram.h"

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
	// Access the singleton instance
	static Graphics& Instance();

	// Constants
	static const std::string VERTEX_SHADER_FILE_NAME;
	static const std::string FRAGMENT_SHADER_FILE_NAME;

	static const size_t MAX_CAMERAS;

	static const size_t SCREEN_WIDTH;
	static const size_t SCREEN_HEIGHT;

	static const glm::vec3 SKY_COLOR;
	static const glm::vec3 AMBIENT_COLOR;

	// System calls
	bool Initialize(char* windowTitle);
	void Update(Time deltaTime) override;

	// System accessors
	GLFWwindow* GetWindow() const;

	static void WindowSizeCallback(GLFWwindow *window, int width, int height);
	void SetWindowDimensions(size_t width, size_t height);
	float GetViewportAspectRatio() const;
	glm::vec2 GetViewportDimensions() const;
	void UpdateViewports();

private:
	// No instantiation or copying
	Graphics();
	Graphics(const Graphics&) = delete;
	Graphics& operator= (const Graphics&) = delete;

	void CountCameras();
	size_t cameraCount;
	std::vector<Component*> cameras;
	
	GLFWwindow* window;
	size_t windowWidth;
	size_t windowHeight;
	
	GLuint vboIds[VBOs::Count];		// Points and UVs
	GLuint vaoIds[VAOs::Count];
	ShaderProgram* shaders[Shaders::Count];

    void LoadTexture(GLuint textureId, const char *uniformName);
    void LoadTexture(Texture *texture, std::string uniformName);

	void LoadBuffer(const glm::vec3 *vertices, const glm::vec2 *uvs, const glm::vec3 *normals, const size_t vertexCount);
	void LoadBuffer(Mesh *mesh);

	void DestroyIds();
	void GenerateIds();

	void InitializeVao();
	ShaderProgram* LoadShaderProgram();
};
