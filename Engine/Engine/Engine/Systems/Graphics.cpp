#include "Graphics.h"
#include "IO/Mouse.h"
#include "IO/Keyboard.h"

#include <iostream>
#include "Content/ContentManager.h"

// Constants
const size_t Graphics::MAX_CAMERAS = 4;

// Shader paths
const std::string Graphics::VERTEX_SHADER_FILE_NAME = "vertex.glsl";
const std::string Graphics::FRAGMENT_SHADER_FILE_NAME = "fragment.glsl";

//Screen Dimensions Values
int Graphics::SCREEN_WIDTH = 1024;
int Graphics::SCREEN_HEIGHT = 768;

//GLFW Window
GLFWwindow* Graphics::window = NULL;
GLuint vertexbuffer;

bool Graphics::Initialize(char* windowTitle) {
	if (!glfwInit()) {
		std::cout << "Error Initializing GLFW" << std::endl;
		return false;
	}

	//version check for openGL

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

							//Input Callbacks
	glfwSetCursorPosCallback(window, Mouse::MousePositionCallback);
	glfwSetMouseButtonCallback(window, Mouse::MouseButtonCallback);
	glfwSetKeyCallback(window, Keyboard::KeyboardCallback);
	//glfwSetJoystickCallback(Controller::ControllerCallback);

	// Centers window
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	int xPos = (mode->width - SCREEN_WIDTH) / 2;
	int yPos = (mode->height - SCREEN_HEIGHT) / 2;
	glfwSetWindowPos(window, xPos, yPos);

	//clear the screen to grey
	glClearColor(.1f, .1f, .1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//GL Setup
	//Viewport
	glfwGetWindowSize(window, &width, &height); //check resize
	windowWidth = width;
	windowHeight = height;

	/*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -10, 10);
	glDepthRange(-10, 10);
	glMatrixMode(GL_MODELVIEW);*/
	//glFrustum(-1, 1, -1, 1, 0, 100);

	//Alpha Blending
	/*glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/

	// Z-Buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glewExperimental = GL_TRUE;		// TODO: Determine whether this is necessary or not
	glewInit();
	GenerateIds();
	InitializeVao();

	return true;
}

void Graphics::Update(Time deltaTime) {
	glfwPollEvents();

	//Clear Back Buffer Before We Draw
	glClearColor(.1f, .1f, .1f, 1.0f);			// TODO: I think this only needs to be set once
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderIds[Shaders::Program]);
	glBindVertexArray(vaoIds[VAOs::Vertices]);

	for (size_t i = 0; i < cameras.size(); i++) {
		glViewport(0, 0, windowWidth, windowHeight);	// TODO: Calculate viewport pos/size for each camera
		CameraComponent* camera = cameras[i];
		const glm::mat4 projectionMatrix = camera->GetProjectionMatrix();
		const glm::mat4 viewMatrix = camera->GetViewMatrix();
		for (size_t j = 0; j < meshComponents.size(); j++) {
			MeshComponent* meshComponent = meshComponents[j];
			glm::mat4 modelMatrix = meshComponent->transform.GetTransformationMatrix();
			glm::mat4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;

			GLuint lightColorId = glGetUniformLocation(shaderIds[Shaders::Program], "lightColor");
			glUniform3f(lightColorId, 1.f, 1.f, 1.f);

			GLuint lightPowerId = glGetUniformLocation(shaderIds[Shaders::Program], "lightPower");
			glUniform1f(lightPowerId, 50.f);

			GLuint lightPositionId = glGetUniformLocation(shaderIds[Shaders::Program], "lightPosition_world");
			glUniform3f(lightPositionId, 0.f, 0.f, -5);

			GLuint modelMatrixId = glGetUniformLocation(shaderIds[Shaders::Program], "modelMatrix");
			glUniformMatrix4fv(modelMatrixId, 1, GL_FALSE, &modelMatrix[0][0]);

			GLuint viewMatrixId = glGetUniformLocation(shaderIds[Shaders::Program], "viewMatrix");
			glUniformMatrix4fv(viewMatrixId, 1, GL_FALSE, &viewMatrix[0][0]);

			GLuint mvpMatrixId = glGetUniformLocation(shaderIds[Shaders::Program], "modelViewProjectionMatrix");
			glUniformMatrix4fv(mvpMatrixId, 1, GL_FALSE, &modelViewProjectionMatrix[0][0]);

            Material *mat = meshComponent->material;
            
			GLuint diffuseId = glGetUniformLocation(shaderIds[Shaders::Program], "materialDiffuseColor");
			glUniform3f(diffuseId, mat->diffuseColor.r, mat->diffuseColor.g, mat->diffuseColor.b);

            GLuint ambientId = glGetUniformLocation(shaderIds[Shaders::Program], "materialAmbientColorMultiplier");
			glUniform3f(ambientId, mat->ambientColor.r, mat->ambientColor.g, mat->ambientColor.b);

            GLuint specularId = glGetUniformLocation(shaderIds[Shaders::Program], "materialSpecularColor");
			glUniform3f(specularId, mat->specularColor.r, mat->specularColor.g, mat->specularColor.b);

            GLuint specularityId = glGetUniformLocation(shaderIds[Shaders::Program], "materialSpecularity");
			glUniform1f(specularityId, mat->specularity);
			
			Mesh* mesh = meshComponent->GetMesh();
			LoadBuffer(mesh);
            LoadTexture(meshComponent->texture, "diffuseTexture");
			glDrawArrays(GL_TRIANGLES, 0, mesh->vertexCount);
		}
	}

	//Swap Buffers to Display New Frame
	glfwSwapBuffers(window);
}

GLFWwindow* Graphics::GetWindow() const {
	return window;
}

bool Graphics::RegisterCamera(CameraComponent* camera) {
	if (cameras.size() > MAX_CAMERAS) {
		return false;
	}
	cameras.push_back(camera);
	camera->SetAspectRatio(static_cast<float>(windowWidth) / static_cast<float>(windowHeight));		// TODO: Update in relevant locations
	return true;
}

void Graphics::UnregisterCamera(CameraComponent* camera) {
	auto it = find(cameras.begin(), cameras.end(), camera);
	if (it != cameras.end())
		cameras.erase(it);
}

void Graphics::LoadTexture(GLuint textureId, const char *uniformName) {
    glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);
	
	GLuint uniformLocation = glGetUniformLocation(shaderIds[Shaders::Program], uniformName);
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
}

void Graphics::DestroyIds() {
	for (int i = 0; i < Shaders::Count; i++) {
		glDeleteProgram(shaderIds[i]);
	}
	glDeleteVertexArrays(VAOs::Count, vaoIds);
	glDeleteBuffers(VBOs::Count, vboIds);
}

void Graphics::GenerateIds() {
	glGenVertexArrays(VAOs::Count, vaoIds);
	glGenBuffers(VBOs::Count, vboIds);
	shaderIds[Shaders::Program] = LoadShaderProgram();
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
}

GLuint Graphics::LoadShaderProgram() {
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
	return programId;
}
