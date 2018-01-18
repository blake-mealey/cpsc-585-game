#pragma once

#include "glm/glm.hpp"

struct Material {
    Material(glm::vec3 _diffuseColor, glm::vec3 _specularColor, float _specularity) :
      diffuseColor(_diffuseColor), specularColor(_specularColor), specularity(_specularity) {}

	glm::vec3 diffuseColor;
	glm::vec3 specularColor;

	float specularity;
};
