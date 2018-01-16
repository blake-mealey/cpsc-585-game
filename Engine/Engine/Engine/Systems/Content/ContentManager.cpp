#include "ContentManager.h"
#include <fstream>
#include <iterator>
#include <iostream>
#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

std::map<std::string, Mesh*> ContentManager::meshes;
std::map<std::string, Texture*> ContentManager::textures;
std::map<std::string, Material*> ContentManager::materials;

const std::string ContentManager::CONTENT_DIR_PATH = "./Content/";
const std::string ContentManager::MESH_DIR_PATH = CONTENT_DIR_PATH + "Meshes/";
const std::string ContentManager::TEXTURE_DIR_PATH = CONTENT_DIR_PATH + "Textures/";
const std::string ContentManager::MATERIAL_DIR_PATH = CONTENT_DIR_PATH + "Materials/";

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

Mesh* LoadObj(const std::string filePath) {
	FILE *file = fopen(filePath.c_str(), "r");
	if (file == NULL) {
		std::cout << "Failed to load OBJ file." << std::endl;
		return nullptr;
	}

	std::vector<glm::vec3> tempVertices;
	std::vector<glm::vec2> tempUvs;
	std::vector<glm::vec3> tempNormals;

	std::vector<unsigned int> vertexIndices;
	std::vector<unsigned int> uvIndices;
	std::vector<unsigned int> normalIndices;

	while (true) {
		char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF) {
			break;
		}

		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			tempVertices.push_back(vertex);
		} else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			tempUvs.push_back(uv);
		} else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			tempNormals.push_back(normal);
		} else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
				&vertexIndex[0], &uvIndex[0], &normalIndex[0],
				&vertexIndex[1], &uvIndex[1], &normalIndex[1],
				&vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				std::cout << "Unknown format for simple parser." << std::endl;
				return nullptr;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}

	const size_t vertexCount = vertexIndices.size();
	glm::vec3 *vertices = new glm::vec3[vertexCount];
	glm::vec2 *uvs = new glm::vec2[vertexCount];
	glm::vec3 *normals = new glm::vec3[vertexCount];

	for (size_t i = 0; i < vertexCount; i++) {
		vertices[i] = tempVertices[vertexIndices[i] - 1];
		uvs[i] = tempUvs[uvIndices[i] - 1];
		normals[i] = tempNormals[normalIndices[i] - 1];
	}

	return new Mesh(vertices, uvs, normals, vertexCount);
}

Mesh* ContentManager::GetMesh(const std::string filePath) {
	Mesh* mesh = meshes[filePath];
	if (mesh != nullptr) {
		return mesh;
	}

	// TODO: Replace with loading mesh from path: MESH_DIR_PATH + filePath
//	mesh = GenerateCube(1.f);
//	mesh = GenerateTriangle();
	mesh = LoadObj(MESH_DIR_PATH + filePath);

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

Material* ContentManager::GetMaterial(const std::string filePath) {
	Material* material = materials[filePath];
	if (material != nullptr) {
		return material;
	}

	nlohmann::json data = LoadJson(filePath);
	const glm::vec3 diffuseColor = glm::vec3(
		data["diffuseColor"][0].get<float>(),
		data["diffuseColor"][1].get<float>(),
		data["diffuseColor"][2].get<float>());
	const glm::vec3 specularColor = glm::vec3(
		data["specularColor"][0].get<float>(),
		data["specularColor"][1].get<float>(),
		data["specularColor"][2].get<float>());
	const glm::vec3 ambientColor = glm::vec3(
		data["ambientColor"][0].get<float>(),
		data["ambientColor"][1].get<float>(),
		data["ambientColor"][2].get<float>());
	const float specularity = data["specularity"].get<float>();

	return new Material(diffuseColor, specularColor, ambientColor, specularity);
}

nlohmann::json ContentManager::LoadJson(const std::string filePath) {
	std::ifstream file(MATERIAL_DIR_PATH + filePath);		// TODO: Error check?
	nlohmann::json object;
	file >> object;
	file.close();
	return object;
}

GLuint ContentManager::LoadShader(std::string filePath, const GLenum shaderType) {
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

	// Create a shader and get it's ID
	const GLuint shaderId = glCreateShader(shaderType);

	// Compile the shader
	const GLchar *sourcePointer = source.c_str();
	glShaderSource(shaderId, 1, &sourcePointer, nullptr);
	glCompileShader(shaderId);

	// Check compile status and print compilation errors
	GLint status;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		GLint length;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
		std::string info(length, ' ');
		glGetShaderInfoLog(shaderId, info.length(), &length, &info[0]);
		std::cout << "ERROR Compiling Shader:" << std::endl << std::endl << source << std::endl << info << std::endl;
	}

	// Return the shader's ID
	return shaderId;
}
