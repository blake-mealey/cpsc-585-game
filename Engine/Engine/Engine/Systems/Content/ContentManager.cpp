#include "ContentManager.h"
#include <fstream>
#include <iterator>
#include <iostream>
#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

std::map<std::string, Mesh*> ContentManager::meshes = std::map<std::string, Mesh*>();
std::map<std::string, Texture*> ContentManager::textures = std::map<std::string, Texture*>();

const std::string ContentManager::CONTENT_DIR_PATH = "./Content/";
const std::string ContentManager::MESH_DIR_PATH = CONTENT_DIR_PATH + "Meshes/";
const std::string ContentManager::TEXTURE_DIR_PATH = CONTENT_DIR_PATH + "Textures/";

const std::string ContentManager::SHADERS_DIR_PATH = "./Engine/Shaders/";

Mesh* GenerateCube(const float sideLength) {
	const int count = 6 * 2 * 3;
	glm::vec3 *vertices = new glm::vec3[count] {
		sideLength * glm::vec3(-1.0f,-1.0f,-1.0f), // triangle 1 : begin
		sideLength * glm::vec3(-1.0f,-1.0f, 1.0f),
		sideLength * glm::vec3(-1.0f, 1.0f, 1.0f), // triangle 1 : end
		sideLength * glm::vec3(1.0f, 1.0f,-1.0f), // triangle 2 : begin
		sideLength * glm::vec3(-1.0f,-1.0f,-1.0f),
		sideLength * glm::vec3(-1.0f, 1.0f,-1.0f), // triangle 2 : end
		sideLength * glm::vec3(1.0f,-1.0f, 1.0f),
		sideLength * glm::vec3(-1.0f,-1.0f,-1.0f),
		sideLength * glm::vec3(1.0f,-1.0f,-1.0f),
		sideLength * glm::vec3(1.0f, 1.0f,-1.0f),
		sideLength * glm::vec3(1.0f,-1.0f,-1.0f),
		sideLength * glm::vec3(-1.0f,-1.0f,-1.0f),
		sideLength * glm::vec3(-1.0f,-1.0f,-1.0f),
		sideLength * glm::vec3(-1.0f, 1.0f, 1.0f),
		sideLength * glm::vec3(-1.0f, 1.0f,-1.0f),
		sideLength * glm::vec3(1.0f,-1.0f, 1.0f),
		sideLength * glm::vec3(-1.0f,-1.0f, 1.0f),
		sideLength * glm::vec3(-1.0f,-1.0f,-1.0f),
		sideLength * glm::vec3(-1.0f, 1.0f, 1.0f),
		sideLength * glm::vec3(-1.0f,-1.0f, 1.0f),
		sideLength * glm::vec3(1.0f,-1.0f, 1.0f),
		sideLength * glm::vec3(1.0f, 1.0f, 1.0f),
		sideLength * glm::vec3(1.0f,-1.0f,-1.0f),
		sideLength * glm::vec3(1.0f, 1.0f,-1.0f),
		sideLength * glm::vec3(1.0f,-1.0f,-1.0f),
		sideLength * glm::vec3(1.0f, 1.0f, 1.0f),
		sideLength * glm::vec3(1.0f,-1.0f, 1.0f),
		sideLength * glm::vec3(1.0f, 1.0f, 1.0f),
		sideLength * glm::vec3(1.0f, 1.0f,-1.0f),
		sideLength * glm::vec3(-1.0f, 1.0f,-1.0f),
		sideLength * glm::vec3(1.0f, 1.0f, 1.0f),
		sideLength * glm::vec3(-1.0f, 1.0f,-1.0f),
		sideLength * glm::vec3(-1.0f, 1.0f, 1.0f),
		sideLength * glm::vec3(1.0f, 1.0f, 1.0f),
		sideLength * glm::vec3(-1.0f, 1.0f, 1.0f),
		sideLength * glm::vec3(1.0f,-1.0f, 1.0f)
	};
    glm::vec2 *uvs = new glm::vec2[count]{
        glm::vec2(0.f, 0.f),
        glm::vec2(1.f, 0.f),
        glm::vec2(1.f, 1.f),
        glm::vec2(0.f, 0.f),
        glm::vec2(0.f, 1.f),
        glm::vec2(1.f, 1.f),
        glm::vec2(0.f, 0.f),
        glm::vec2(1.f, 0.f),
        glm::vec2(1.f, 1.f),
        glm::vec2(0.f, 0.f),
        glm::vec2(0.f, 1.f),
        glm::vec2(1.f, 1.f),
        glm::vec2(0.f, 0.f),
        glm::vec2(1.f, 0.f),
        glm::vec2(1.f, 1.f),
        glm::vec2(0.f, 0.f),
        glm::vec2(0.f, 1.f),
        glm::vec2(1.f, 1.f),
        glm::vec2(0.f, 0.f),
        glm::vec2(1.f, 0.f),
        glm::vec2(1.f, 1.f),
        glm::vec2(0.f, 0.f),
        glm::vec2(0.f, 1.f),
        glm::vec2(1.f, 1.f),
        glm::vec2(0.f, 0.f),
        glm::vec2(1.f, 0.f),
        glm::vec2(1.f, 1.f),
        glm::vec2(0.f, 0.f),
        glm::vec2(0.f, 1.f),
        glm::vec2(1.f, 1.f),
        glm::vec2(0.f, 0.f),
        glm::vec2(1.f, 0.f),
        glm::vec2(1.f, 1.f),
        glm::vec2(0.f, 0.f),
        glm::vec2(0.f, 1.f),
        glm::vec2(1.f, 1.f),
    };
	return new Mesh(vertices, uvs, count);
}

Mesh* GenerateTriangle() {
	glm::vec3 *vertices = new glm::vec3[6] {
		3.f * glm::vec3(0.0f, 0.0f, 0.0f), // triangle 1 : begin
		3.f * glm::vec3(1.0f, 0.0f, 0.0f),
		3.f * glm::vec3(1.0f, 1.0f, 0.0f), // triangle 1 : end
        3.f * glm::vec3(0.0f, 0.0f, 0.0f), // triangle 2 : begin
		3.f * glm::vec3(0.0f, 1.0f, 0.0f),
		3.f * glm::vec3(1.0f, 1.0f, 0.0f), // triangle 2 : end
	};
    glm::vec2 *uvs = new glm::vec2[6]{
        glm::vec2(0.f, 0.f),
        glm::vec2(1.f, 0.f),
        glm::vec2(1.f, 1.f),
        glm::vec2(0.f, 0.f),
        glm::vec2(0.f, 1.f),
        glm::vec2(1.f, 1.f)
    };
	return new Mesh(vertices, uvs, 6);
}

Mesh* ContentManager::GetMesh(const std::string filePath) {
	Mesh* mesh = meshes[filePath];
	if (mesh != nullptr) {
		return mesh;
	}

	// TODO: Replace with loading mesh from path: MESH_DIR_PATH + filePath
	mesh = GenerateCube(0.5f);
//	mesh = GenerateTriangle();

	meshes[filePath] = mesh;
	return mesh;
}

Texture* ContentManager::GetTexture(const std::string filePath) {
	Texture* texture = textures[filePath];
	if (texture != nullptr) {
		return texture;
	}

	int components;
	GLuint texId;
	int tWidth, tHeight;

	const auto data = stbi_load((TEXTURE_DIR_PATH + filePath).c_str(), &tWidth, &tHeight, &components, 0);

	if (data != nullptr) {
		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);

		if (components == 3) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tWidth, tHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		} else if (components == 4) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tWidth, tHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(data);

		texture = new Texture(texId, tWidth, tHeight);
		textures[filePath] = texture;
	}

	return texture;
}

// TODO: Move loading into OpenGL to this function from Graphics and simply return the ID of the shader
std::string ContentManager::LoadShader(std::string filePath) {
	filePath = SHADERS_DIR_PATH + filePath;

	std::string source;
	std::ifstream input(filePath.c_str());
	if (input) {
		copy(std::istreambuf_iterator<char>(input),
			std::istreambuf_iterator<char>(),
			std::back_inserter(source));
		input.close();
	} else {
		std::cout << "ERROR: Could not load shader source from file " << filePath << std::endl;
	}

	return source;
}
