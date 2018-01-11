#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Transform {
public:
	Transform* GetParent();

	glm::mat4 GetLocalTransformationMatrix();
	glm::mat4 GetTransformationMatrix();

	glm::vec3 GetPosition();
	glm::vec3 GetScale();
	glm::quat GetRotation();

	void SetPosition(glm::vec3);
	void SetScale(glm::vec3);
	void SetRotation(glm::quat);

	void Translate(glm::vec3 offset);
	void Scale(float scaleFactor);
	void Rotate(glm::vec3 axis, float radians);
private:
	Transform *parent;
	
	glm::vec3 position;
	glm::vec3 scale;
	glm::quat rotation;

	glm::mat4 translationMatrix;		// For lazy-loading purposes
	glm::mat4 scaleMatrix;
	glm::mat4 rotationMatrix;
	glm::mat4 transformationMatrix;
};