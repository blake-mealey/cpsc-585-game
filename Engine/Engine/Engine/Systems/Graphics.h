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
#include "../Components/PointLightComponent.h"
#include "../Components/DirectionLightComponent.h"
#include "Content/SpotLight.h"

class MeshComponent;

struct Camera {
	Camera(glm::mat4 _viewMatrix, glm::mat4 _projectionMatrix) : viewMatrix(_viewMatrix), projectionMatrix(_projectionMatrix) {}
	
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::vec2 viewportPosition;
	glm::vec2 viewportSize;
};

struct VAOs {
	enum { Vertices=0, Count };
};

struct VBOs {
	enum { Vertices=0, UVs, Normals, Count };
};

struct SSBOs {
	enum { PointLights=0, DirectionLights, SpotLights, Count };
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
	void UpdateViewports(std::vector<Component*> cameraComponents) const;
	glm::vec2 GetViewportSize() const;

private:
	// No instantiation or copying
	Graphics();
	Graphics(const Graphics&) = delete;
	Graphics& operator= (const Graphics&) = delete;

	void DrawMesh(ShaderProgram *shaderProgram, MeshComponent* meshComponent);

	void LoadCameras(std::vector<Component*> cameraComponents);
	std::vector<Camera> cameras;
	
	GLFWwindow* window;
	size_t windowWidth;
	size_t windowHeight;
	
	GLuint vboIds[VBOs::Count];		// Vertices and UV coordinates
	GLuint vaoIds[VAOs::Count];
	GLuint ssboIds[SSBOs::Count];
	ShaderProgram* shaders[Shaders::Count];

	void LoadLights(std::vector<Component*> _pointLights, std::vector<Component*> _directionLights, std::vector<Component*> _spotLights);
	void LoadLights(std::vector<PointLight> pointLights, std::vector<DirectionLight> directionLights, std::vector<SpotLight> spotLights);

    void LoadTexture(GLuint textureId, const char *uniformName);
    void LoadTexture(Texture *texture, std::string uniformName);

	void LoadBuffer(const glm::vec3 *vertices, const glm::vec2 *uvs, const glm::vec3 *normals, const size_t vertexCount);
	void LoadBuffer(Mesh *mesh);

	void DestroyIds();
	void GenerateIds();

	void InitializeVao();
	ShaderProgram* LoadShaderProgram();
};
