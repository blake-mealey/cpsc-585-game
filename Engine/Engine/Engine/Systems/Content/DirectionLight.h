#pragma once

#include <glm/glm.hpp>

struct DirectionLight {
	DirectionLight(glm::vec3 _color, glm::vec3 _direction) : color(_color), direction(_direction) {}

	glm::vec3 color;
	glm::vec3 direction;

	float __padding[2];		// Padding required for passing to the fragment shader via SSBO
};
