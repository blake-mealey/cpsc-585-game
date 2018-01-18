#pragma once

#include "GL/glew.h"
#include <map>

const struct UniformName {
	static const char* LightColor;
	static const char* LightPower;
	static const char* LightPosition_World;

	static const char* AmbientColor;

	static const char* ModelMatrix;
	static const char* ViewMatrix;
	static const char* ModelViewProjectionMatrix;

	static const char* MaterialDiffuseColor;
	static const char* MaterialSpecularColor;
	static const char* MaterialSpecularity;

	static const char* DiffuseTexture;
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
