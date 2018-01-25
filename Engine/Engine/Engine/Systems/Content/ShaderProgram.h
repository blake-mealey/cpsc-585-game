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
