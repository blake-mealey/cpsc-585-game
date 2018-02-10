#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Transform {
public:
	Transform();
	Transform(Transform *pParent, glm::vec3 pPosition, glm::vec3 pScale, glm::quat pRotation, bool connectedToCylinder);

	void Update();

	static float radius;

	Transform *parent;

	static const glm::vec3 FORWARD;
	static const glm::vec3 RIGHT;
	static const glm::vec3 UP;

	// Getters for basic data
	glm::vec3 GetLocalPosition();
	glm::vec3 GetLocalScale();
	glm::quat GetLocalRotation();

	glm::vec3 GetGlobalPosition();
	glm::vec3 Transform::GetGlobalScale();

	glm::vec3 GetCylinderPosition();

	glm::vec3 GetForward();
	glm::vec3 GetRight();
	glm::vec3 GetUp();

	// Setters for basic data
	void SetPosition(glm::vec3 pPosition);
	void SetScale(glm::vec3 pScale);
	void SetRotation(glm::quat pRotation);
	void SetRotationEulerAngles(glm::vec3 eulerAngles);
	void SetRotationAxisAngles(glm::vec3 axis, float radians);


	// Operators for basic data
	void Translate(glm::vec3 offset);
	void Scale(float scaleFactor);
	void Scale(glm::vec3 scaleFactor);
	void Rotate(glm::vec3 axis, float radians);

	// Getters for output data
	glm::mat4 GetTranslationMatrix();
	glm::mat4 GetScalingMatrix();
	glm::mat4 GetRotationMatrix();
	glm::mat4 GetLocalTransformationMatrix();
	glm::mat4 GetTransformationMatrix();

	static glm::vec3 ToCylinder(glm::vec3 point);
	static glm::vec3 FromCylinder(glm::vec3 point);
	void ConnectToCylinder();

	bool connectedToCylinder;
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
