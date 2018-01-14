#pragma once

#include <string>
#include "Mesh.h"
#include <map>
#include "Texture.h"

class ContentManager {
public:
	static const std::string CONTENT_DIR_PATH;
	static const std::string MESH_DIR_PATH;
	static const std::string TEXTURE_DIR_PATH;

	static const std::string SHADERS_DIR_PATH;
	
	static Mesh* GetMesh(std::string filePath);
	static Texture* GetTexture(std::string filePath);
	// TODO: LoadMaterial()

	static std::string LoadShader(std::string filePath);

	// TODO: These guys
	// static Audio*? GetAudio(std::string filePath);
	// static Texture*? GetTexture(std::string filePath);
	// static GameData*? GetDataFile(std::string filePath);

private:
	static std::map<std::string, Mesh*> meshes;
	static std::map<std::string, Texture*> textures;

};
