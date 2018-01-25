#include "ShaderProgram.h"
#include <iostream>

const char* UniformName::AmbientColor = "ambientColor";

const char* UniformName::ModelMatrix = "modelMatrix";
const char* UniformName::ViewMatrix = "viewMatrix";
const char* UniformName::ModelViewProjectionMatrix = "modelViewProjectionMatrix";

const char* UniformName::MaterialDiffuseColor = "materialDiffuseColor";
const char* UniformName::MaterialSpecularColor = "materialSpecularColor";
const char* UniformName::MaterialSpecularity = "materialSpecularity";

const char* UniformName::DiffuseTexture = "diffuseTexture";
const char* UniformName::DiffuseTextureEnabled = "diffuseTextureEnabled";

ShaderProgram::ShaderProgram() {}
ShaderProgram::ShaderProgram(GLuint id) : programId(id) {}

GLuint ShaderProgram::GetId() const {
	return programId;
}

GLuint ShaderProgram::GetUniformLocation(const char* name) {
	const auto it = uniforms.find(name);
	if (it == uniforms.end()) {
		uniforms[name] = glGetUniformLocation(programId, name);
	}
	return it->second;
}
