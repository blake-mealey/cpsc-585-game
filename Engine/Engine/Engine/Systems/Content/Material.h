#pragma once

#include "glm/glm.hpp"

struct Material {
    Material(glm::vec3 _diffuseColor, glm::vec3 _specularColor, glm::vec3 _ambientColor, float _specularity) :
      diffuseColor(_diffuseColor), specularColor(_specularColor),
      ambientColor(_ambientColor), specularity(_specularity) {}

	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	glm::vec3 ambientColor;

	float specularity;
};
