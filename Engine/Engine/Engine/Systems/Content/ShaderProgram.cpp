#include "ShaderProgram.h"
#include <iostream>

const char* UniformName::LightColor = "lightColor";
const char* UniformName::LightPower = "lightPower";
const char* UniformName::LightPosition_World = "lightPosition_world";

const char* UniformName::AmbientColor = "ambientColor";

const char* UniformName::ModelMatrix = "modelMatrix";
const char* UniformName::ViewMatrix = "viewMatrix";
const char* UniformName::ModelViewProjectionMatrix = "modelViewProjectionMatrix";

const char* UniformName::MaterialDiffuseColor = "materialDiffuseColor";
const char* UniformName::MaterialSpecularColor = "materialSpecularColor";
const char* UniformName::MaterialSpecularity = "materialSpecularity";

const char* UniformName::DiffuseTexture = "diffuseTexture";

ShaderProgram::ShaderProgram() {}
ShaderProgram::ShaderProgram(GLuint id) : programId(id) {}

GLuint ShaderProgram::GetId() const {
	return programId;
}

GLuint ShaderProgram::GetUniformLocation(const char* name) {
	auto it = uniforms.find(name);
	if (it == uniforms.end()) {
		GLuint location = glGetUniformLocation(programId, name);
		uniforms[name] = location;
	}
	return it->second;
}
