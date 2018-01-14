#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Transform {
public:
	Transform();
	Transform(Transform *pParent, glm::vec3 pPosition, glm::vec3 pScale, glm::quat pRotation);

	Transform *parent;

	// Getters for basic data
	glm::vec3 GetPosition();
	glm::vec3 GetScale();
	glm::quat GetRotation();

	// Setters for basic data
	void SetPosition(glm::vec3 pPosition);
	void SetScale(glm::vec3 pScale);
	void SetRotation(glm::quat pRotation);
	void SetRotationEulerAngles(glm::vec3 eulerAngles);
	void SetRotationAxisAngles(glm::vec3 axis, float radians);

	// Operators for basic data
	void Translate(glm::vec3 offset);
	void Scale(float scaleFactor);
	void Rotate(glm::vec3 axis, float radians);

	// Getters for output data
	glm::mat4 GetTranslationMatrix();
	glm::mat4 GetScalingMatrix();
	glm::mat4 GetRotationMatrix();
	glm::mat4 GetLocalTransformationMatrix();
	glm::mat4 GetTransformationMatrix();
private:
	// Basic data
	glm::vec3 position;
	glm::vec3 scale;
	glm::quat rotation;

	// Output data for lazy-loading purposes
	glm::mat4 translationMatrix;
	glm::mat4 scalingMatrix;
	glm::mat4 rotationMatrix;
	glm::mat4 transformationMatrix;

	void UpdateTransformationMatrix();
};
