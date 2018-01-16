#pragma once

#include <glm/glm.hpp>

class Mesh {
public:
	Mesh(glm::vec3 *_vertices, glm::vec2 *_uvs, glm::vec3 *_normals, size_t _vertexCount);
	Mesh(glm::vec3 *_vertices, glm::vec2 *_uvs, size_t _vertexCount);

	const glm::vec3 *vertices;
	const glm::vec2 *uvs;
	const glm::vec3 *normals;

	const size_t vertexCount;

private:
	void GenerateNormals();
};
