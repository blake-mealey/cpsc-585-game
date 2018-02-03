#pragma once

#include "GL/glew.h"
#include <map>

const struct UniformName {
	static const char* AmbientColor;

	static const char* ModelMatrix;
	static const char* ViewMatrix;
	static const char* ModelViewProjectionMatrix;

	static const char* MaterialDiffuseColor;
	static const char* MaterialSpecularColor;
	static const char* MaterialSpecularity;

	static const char* DiffuseTexture;
	static const char* DiffuseTextureEnabled;
	static const char* UvScale;
	
	static const char* ShadowMap;
	static const char* DepthModelViewProjectionMatrix;
	static const char* DepthBiasModelViewProjectionMatrix;
	
    static const char* Skybox;
    static const char* ViewProjectionMatrix;
};

class ShaderProgram {
public:
	ShaderProgram();
	ShaderProgram(GLuint id);

	GLuint GetId() const;

	GLuint GetUniformLocation(const char* name);

private:
	GLuint programId;
	std::map<std::string, GLuint> uniforms;
};
