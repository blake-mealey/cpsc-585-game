#pragma once

#include <string>
#include "Mesh.h"
#include <map>
#include "Texture.h"
#include "Material.h"
#include "json/json.hpp"

struct Texture;

class ContentManager {
public:
	static const std::string CONTENT_DIR_PATH;
	static const std::string MESH_DIR_PATH;
	static const std::string TEXTURE_DIR_PATH;
	static const std::string MATERIAL_DIR_PATH;

	static const std::string SHADERS_DIR_PATH;

	static Mesh* GetMesh(std::string filePath);
	static Texture* GetTexture(std::string filePath);
	static Material* GetMaterial(std::string filePath);

	static nlohmann::json LoadJson(std::string filePath);

	static GLuint LoadShader(std::string filePath, GLenum shaderType);

	// TODO: These guys
	// static Audio*? GetAudio(std::string filePath);
	// static Texture*? GetTexture(std::string filePath);
	// static GameData*? GetDataFile(std::string filePath);

private:
	static std::map<std::string, Mesh*> meshes;
	static std::map<std::string, Texture*> textures;
	static std::map<std::string, Material*> materials;

};
