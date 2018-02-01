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
	enum { Geometry=0, ShadowMap, Count };
};

struct VBOs {
	enum { Vertices=0, UVs, Normals, Count };
};

struct SSBOs {
	enum { PointLights=0, DirectionLights, SpotLights, Count };
};

struct FBOs {
	enum { ShadowMap=0, Count };
};

struct Textures {
	enum { ShadowMap=0, Count };
};

struct Shaders {
	enum { Geometry=0, ShadowMap, Count };
};

class Graphics : public System {
public:
	// Access the singleton instance
	static Graphics& Instance();

	// Constants
	static const std::string GEOMETRY_VERTEX_SHADER;
	static const std::string GEOMETRY_FRAGMENT_SHADER;
	static const std::string SHADOW_MAP_VERTEX_SHADER;
	static const std::string SHADOW_MAP_FRAGMENT_SHADER;

	static const size_t MAX_CAMERAS;

	static const size_t SCREEN_WIDTH;
	static const size_t SCREEN_HEIGHT;
	static const size_t SHADOW_MAP_SIZE;

	static const glm::vec3 SKY_COLOR;
	static const glm::vec3 AMBIENT_COLOR;

	static const glm::mat4 BIAS_MATRIX;

	// System calls
	bool Initialize(char* windowTitle);
	void Update(Time currentTime, Time deltaTime) override;

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

	void LoadModel(ShaderProgram* shaderProgram, MeshComponent* model);

	void LoadCameras(std::vector<Component*> cameraComponents);
	std::vector<Camera> cameras;
	
	GLFWwindow* window;
	size_t windowWidth;
	size_t windowHeight;
	
	GLuint vboIds[VBOs::Count];		// Geometry and UV coordinates
	GLuint vaoIds[VAOs::Count];
	GLuint ssboIds[SSBOs::Count];
	GLuint fboIds[FBOs::Count];
public:
	GLuint textureIds[Textures::Count];
private:
	ShaderProgram* shaders[Shaders::Count];

	void LoadLights(std::vector<Component*> _pointLights, std::vector<Component*> _directionLights, std::vector<Component*> _spotLights);
	void LoadLights(std::vector<PointLight> pointLights, std::vector<DirectionLight> directionLights, std::vector<SpotLight> spotLights);

    void LoadTexture(GLuint uniformLocation, GLuint textureId);
    void LoadTexture(ShaderProgram *program, Texture *texture, std::string uniformName);

	void LoadMesh(Mesh *mesh);
	void LoadVertices(const glm::vec3 *vertices, const size_t vertexCount);
	void LoadUvs(const glm::vec2 *uvs, const size_t vertexCount);
	void LoadNormals(const glm::vec3 *normals, const size_t vertexCount);

	void DestroyIds();
	void GenerateIds();

	void InitializeGeometryVao();
	void InitializeShadowMapVao();
	void InitializeShadowMapFramebuffer();
	ShaderProgram* LoadShaderProgram(std::string vertexShaderFile, std::string fragmentShaderFile) const;
};
