#include "Graphics.h"
#include "IO/Mouse.h"
#include "IO/Keyboard.h"

#include <iostream>
#include "Content/ContentManager.h"
#include <glm/gtx/string_cast.hpp>
#include "../Entities/EntityManager.h"
#include "../Components/CameraComponent.h"
#include "../Components/MeshComponent.h"

// Constants
const size_t Graphics::MAX_CAMERAS = 4;

// Shader paths
const std::string Graphics::VERTEX_SHADER_FILE_NAME = "vertex.glsl";
const std::string Graphics::FRAGMENT_SHADER_FILE_NAME = "fragment.glsl";

// Initial Screen Dimensions
const size_t Graphics::SCREEN_WIDTH = 1024;
const size_t Graphics::SCREEN_HEIGHT = 768;

// Lighting
const glm::vec3 Graphics::SKY_COLOR = glm::vec3(144.f, 195.f, 212.f) / 255.f;
const glm::vec3 Graphics::AMBIENT_COLOR = glm::vec3(0.4f);

// Singleton
Graphics::Graphics() { }
Graphics &Graphics::Instance() {
	static Graphics instance;
	return instance;
}

void Graphics::WindowSizeCallback(GLFWwindow *window, int width, int height) {
	Instance().SetWindowDimensions(width, height);
}

bool Graphics::Initialize(char* windowTitle) {
	if (!glfwInit()) {
		std::cout << "Error Initializing GLFW" << std::endl;
		return false;
	}

	//Create Window
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, windowTitle, NULL, NULL);
	if (window == NULL) {
		std::cout << "Error Creating Window terminate" << std::endl;
		return false;
	}

	//GLFW Setup
	glfwMakeContextCurrent(window);
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glfwSwapInterval(1);	//Swap Buffer Every Frame (Double Buffering)

	// Input callbacks
	glfwSetCursorPosCallback(window, Mouse::MousePositionCallback);
	glfwSetMouseButtonCallback(window, Mouse::MouseButtonCallback);
	glfwSetKeyCallback(window, Keyboard::KeyboardCallback);
	//glfwSetJoystickCallback(Controller::ControllerCallback);

	// Window callbacks
	glfwSetWindowSizeCallback(window, Graphics::WindowSizeCallback);

	// Centers window
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	int xPos = (mode->width - SCREEN_WIDTH) / 2;
	int yPos = (mode->height - SCREEN_HEIGHT) / 2;
	glfwSetWindowPos(window, xPos, yPos);

	//GL Setup
	//Viewport
	glfwGetWindowSize(window, &width, &height); //check resize
	windowWidth = width;
	windowHeight = height;

	//Alpha Blending
	/*glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/

	// Z-Buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Sets the sky color
	glClearColor(SKY_COLOR.r, SKY_COLOR.g, SKY_COLOR.b, 1.0f);

	glewExperimental = GL_TRUE;		// TODO: Determine whether this is necessary or not
	glewInit();
	GenerateIds();
	InitializeVao();

	return true;
}

void Graphics::Update(Time deltaTime) {
	glfwPollEvents();

	//Clear Back Buffer Before We Draw
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set the current shader and VAO
	ShaderProgram *shaderProgram = shaders[Shaders::Program];
	glUseProgram(shaderProgram->GetId());
	glBindVertexArray(vaoIds[VAOs::Vertices]);

	// Load our lights into the GPU
	glUniform3f(shaderProgram->GetUniformLocation(UniformName::AmbientColor), AMBIENT_COLOR.r, AMBIENT_COLOR.g, AMBIENT_COLOR.b);
	const std::vector<Component*> pointLights = EntityManager::GetComponents(ComponentType_PointLight);
	const std::vector<Component*> directionLights = EntityManager::GetComponents(ComponentType_DirectionLight);
	LoadLights(pointLights, directionLights);

	// Get components
	LoadCameras(EntityManager::GetComponents(ComponentType_Camera));
	std::vector<Component*> meshes = EntityManager::GetComponents(ComponentType_Mesh);
	
	// Draw the scene
	for (size_t j = 0; j < meshes.size(); j++) {
		MeshComponent* meshComponent = static_cast<MeshComponent*>(meshes[j]);
		DrawMesh(shaderProgram, meshComponent);
	}

	//Swap Buffers to Display New Frame
	glfwSwapBuffers(window);
}

void Graphics::DrawMesh(ShaderProgram *shaderProgram, MeshComponent* meshComponent) {
	if (!meshComponent->enabled) return;

	// Load the model matrix into the GPU
	glm::mat4 modelMatrix = meshComponent->transform.GetTransformationMatrix();
	glUniformMatrix4fv(shaderProgram->GetUniformLocation(UniformName::ModelMatrix), 1, GL_FALSE, &modelMatrix[0][0]);

	// Get the mesh's material
	Material *mat = meshComponent->material;

	// Load the material data into the GPU
	glUniform3f(shaderProgram->GetUniformLocation(UniformName::MaterialDiffuseColor), mat->diffuseColor.r, mat->diffuseColor.g, mat->diffuseColor.b);
	glUniform3f(shaderProgram->GetUniformLocation(UniformName::MaterialSpecularColor), mat->specularColor.r, mat->specularColor.g, mat->specularColor.b);
	glUniform1f(shaderProgram->GetUniformLocation(UniformName::MaterialSpecularity), mat->specularity);

	// Load the mesh and the mesh's texture into the GPU
	Mesh* mesh = meshComponent->GetMesh();
	LoadBuffer(mesh);

	if (meshComponent->texture != nullptr) {
		glUniform1ui(shaderProgram->GetUniformLocation(UniformName::DiffuseTextureEnabled), 1);
		LoadTexture(meshComponent->texture, UniformName::DiffuseTexture);
	} else {
		glUniform1ui(shaderProgram->GetUniformLocation(UniformName::DiffuseTextureEnabled), 0);
	}

	for (Camera camera : cameras) {
		glViewport(camera.viewportPosition.x, camera.viewportPosition.y, camera.viewportSize.x, camera.viewportSize.y);

		// Load the model view projection matrix into the GPU
		glm::mat4 modelViewProjectionMatrix = camera.projectionMatrix * camera.viewMatrix * modelMatrix;
		glUniformMatrix4fv(shaderProgram->GetUniformLocation(UniformName::ViewMatrix), 1, GL_FALSE, &camera.viewMatrix[0][0]);
		glUniformMatrix4fv(shaderProgram->GetUniformLocation(UniformName::ModelViewProjectionMatrix), 1, GL_FALSE, &modelViewProjectionMatrix[0][0]);

		glDrawArrays(GL_TRIANGLES, 0, mesh->vertexCount);
	}
}

void Graphics::LoadCameras(std::vector<Component*> cameraComponents) {
	// Find up to MAX_CAMERAS enabled cameras
	const size_t lastCount = cameras.size();
	cameras.clear();
	for (Component *component: cameraComponents) {
		if (component->enabled) {
			CameraComponent *camera = static_cast<CameraComponent*>(component);
			cameras.push_back(Camera(camera->GetViewMatrix(), camera->GetProjectionMatrix()));
			if (cameras.size() == MAX_CAMERAS) break;
		}
	}
	const size_t count = cameras.size();

	// Update the camera's viewports based on the number of cameras
	const glm::vec2 windowSize = glm::vec2(windowWidth, windowHeight);
	const glm::vec2 viewportSize = GetViewportSize();
	for (size_t i = 0; i < count; ++i) {
		cameras[i].viewportPosition = glm::vec2((i % 2) * 0.5f,
			i < 2 ? (count < 3 ? 0.f : 0.5f) : 0.f) * windowSize;
		cameras[i].viewportSize = viewportSize;
	}

	// If camera count changed, update aspect ratios for next frame
	if (cameras.size() != lastCount) {
		UpdateViewports(cameraComponents);
	}
}

GLFWwindow* Graphics::GetWindow() const {
	return window;
}

void Graphics::SetWindowDimensions(size_t width, size_t height) {
	windowWidth = width;
	windowHeight = height;
	UpdateViewports(EntityManager::GetComponents(ComponentType_Camera));
}

void Graphics::UpdateViewports(std::vector<Component*> cameraComponents) const {
	const glm::vec2 viewportSize = GetViewportSize();
	const float aspectRatio = viewportSize.x / viewportSize.y;
	for (Component *component : cameraComponents) {
		if (component->enabled) {
			CameraComponent *camera = static_cast<CameraComponent*>(component);
			camera->SetAspectRatio(aspectRatio);
		}
	}
}

glm::vec2 Graphics::GetViewportSize() const {
	const glm::vec2 windowSize = glm::vec2(windowWidth, windowHeight);
	return glm::vec2(cameras.size() == 1 ? 1 : 0.5, cameras.size() < 3 ? 1 : 0.5) * windowSize;
}

void Graphics::LoadLights(std::vector<Component*> _pointLights,
	std::vector<Component*> _directionLights) {

	// Get the point light data which can be directly passed to the shader	
	std::vector<PointLight> pointLights;
	for (Component *component : _pointLights) {
		if (component->enabled)
			pointLights.push_back(static_cast<PointLightComponent*>(component)->GetData());
	}

	// Get the direction light data which can be directly passed to the shader
	std::vector<DirectionLight> directionLights;
	for (Component *component : _directionLights) {
		if (component->enabled)
			directionLights.push_back(static_cast<DirectionLightComponent*>(component)->GetData());
	}

	LoadLights(pointLights, directionLights);
}

void Graphics::LoadLights(std::vector<PointLight> pointLights, std::vector<DirectionLight> directionLights) {
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboIds[SSBOs::PointLights]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, pointLights.size() * sizeof(PointLight), pointLights.data(), GL_DYNAMIC_COPY);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboIds[SSBOs::DirectionLights]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, directionLights.size() * sizeof(DirectionLight), directionLights.data(), GL_DYNAMIC_COPY);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void Graphics::LoadTexture(GLuint textureId, const char *uniformName) {
    glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);
	
	GLuint uniformLocation = glGetUniformLocation(shaders[Shaders::Program]->GetId(), uniformName);
	glUniform1i(uniformLocation, 0);
}

void Graphics::LoadTexture(Texture *texture, std::string uniformName) {
    LoadTexture(texture->textureId, uniformName.c_str());
}

void Graphics::LoadBuffer(Mesh* mesh) {
	LoadBuffer(mesh->vertices, mesh->uvs, mesh->normals, mesh->vertexCount);
}

void Graphics::LoadBuffer(const glm::vec3 *vertices, const glm::vec2 *uvs, const glm::vec3 *normals, const size_t vertexCount) {
	glBindBuffer(GL_ARRAY_BUFFER, vboIds[VBOs::Vertices]);
	glBufferData(
		GL_ARRAY_BUFFER,					// Which buffer to load into
		sizeof(glm::vec3) * vertexCount,	// Size of data in array (bytes)
		vertices,							// Start of array
		GL_DYNAMIC_DRAW						// GL_STATIC_DRAW if seldom changing, GL_DYNAMIC_DRAW if frequently changing
	);

	glBindBuffer(GL_ARRAY_BUFFER, vboIds[VBOs::UVs]);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(glm::vec2) * vertexCount,
		uvs,
		GL_DYNAMIC_DRAW
	);

	glBindBuffer(GL_ARRAY_BUFFER, vboIds[VBOs::Normals]);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(glm::vec3) * vertexCount,
		normals,
		GL_DYNAMIC_DRAW
	);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Graphics::DestroyIds() {
	for (int i = 0; i < Shaders::Count; i++) {
		glDeleteProgram(shaders[i]->GetId());
	}
	glDeleteVertexArrays(VAOs::Count, vaoIds);
	glDeleteBuffers(VBOs::Count, vboIds);
}

void Graphics::GenerateIds() {
	glGenVertexArrays(VAOs::Count, vaoIds);
	glGenBuffers(VBOs::Count, vboIds);
	shaders[Shaders::Program] = LoadShaderProgram();
	glGenBuffers(SSBOs::Count, ssboIds);
	for (size_t i = 0; i < SSBOs::Count; i++) {
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, i, ssboIds[i]);
	}
}

void Graphics::InitializeVao() {
	glBindVertexArray(vaoIds[VAOs::Vertices]);

	// Vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vboIds[VBOs::Vertices]);
	glVertexAttribPointer(
		0,							// Attribute
		3,							// Size (#Components)
		GL_FLOAT,					// Type
		GL_FALSE,					// Normalized?
		0,							// Stride
		static_cast<void*>(nullptr)	// Offset
	);

	// UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vboIds[VBOs::UVs]);
	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		static_cast<void*>(nullptr)
	);

	// Normals
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, vboIds[VBOs::Normals]);
	glVertexAttribPointer(
		2,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		static_cast<void*>(nullptr)
	);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

ShaderProgram* Graphics::LoadShaderProgram() {
	// Load and compile shaders from source
	GLuint vertexId = ContentManager::LoadShader(VERTEX_SHADER_FILE_NAME, GL_VERTEX_SHADER);
	GLuint fragmentId = ContentManager::LoadShader(FRAGMENT_SHADER_FILE_NAME, GL_FRAGMENT_SHADER);

	// Link the shaders into a program
	GLuint programId = glCreateProgram();
	glAttachShader(programId, vertexId);
	glAttachShader(programId, fragmentId);
	glLinkProgram(programId);

	// Check link status and print errors
	GLint status;
	glGetProgramiv(programId, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		GLint length;
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &length);
		std::string info(length, ' ');
		glGetProgramInfoLog(programId, info.length(), &length, &info[0]);
		std::cout << "ERROR linking shader program:" << std::endl << info << std::endl;
	}

	// Return the program's ID
	return new ShaderProgram(programId);
}
