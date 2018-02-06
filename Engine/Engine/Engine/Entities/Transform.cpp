#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

float Transform::radius = 0;

Transform::Transform() : Transform(nullptr, glm::vec3(), glm::vec3(1.f, 1.f, 1.f), glm::quat(), false) {}

Transform::Transform(Transform *pParent, glm::vec3 pPosition, glm::vec3 pScale, glm::quat pRotation, bool connected) : parent(pParent) {
	connectedToCylinder = connected;
	SetPosition(pPosition);
	SetScale(pScale);
	SetRotation(pRotation);
}

void Transform::Update() {
	SetPosition(position);
	SetScale(scale);
	SetRotation(rotation);
}

void Transform::ConnectToCylinder() {
	connectedToCylinder = true;
}

glm::vec3 Transform::GetPosition() {
	return position;
}

glm::vec3 Transform::GetScale() {
	return scale;
}

glm::quat Transform::GetRotation() {
	return rotation;
}

void Transform::UpdateTransformationMatrix() {
	transformationMatrix = translationMatrix * rotationMatrix * scalingMatrix;
}

void Transform::SetPosition(glm::vec3 pPosition) {
	position = pPosition;
	if (connectedToCylinder && radius > 0) {
		translationMatrix = glm::translate(glm::mat4(), ToCylinder(pPosition));
		//rotate accordingly
		float rotBy = position.x / radius;
		rotationMatrix = glm::toMat4(glm::rotate(rotation, rotBy, glm::vec3(0,0,1)));
	}
	else {
		translationMatrix = glm::translate(glm::mat4(), position);
	}
	UpdateTransformationMatrix();
}

void Transform::SetScale(glm::vec3 pScale) {
	scale = pScale;
	scalingMatrix = glm::scale(scale);
	UpdateTransformationMatrix();
}

void Transform::SetRotation(glm::quat pRotation) {
	rotation = pRotation;
	rotationMatrix = glm::toMat4(rotation);
	UpdateTransformationMatrix();
}

void Transform::SetRotationEulerAngles(glm::vec3 eulerAngles) {
	SetRotation(glm::quat(eulerAngles));
}

void Transform::SetRotationAxisAngles(glm::vec3 axis, float radians) {
	SetRotation(glm::angleAxis(radians, axis));
}

void Transform::Translate(glm::vec3 offset) {
	SetPosition(position + offset);
}

void Transform::Scale(float scaleFactor) {
	SetScale(scale * scaleFactor);
}

void Transform::Rotate(glm::vec3 axis, float radians) {
	SetRotation(glm::rotate(rotation, glm::degrees(radians), axis));
}


glm::mat4 Transform::GetTranslationMatrix() {
	return translationMatrix;
}

glm::mat4 Transform::GetScalingMatrix() {
	return scalingMatrix;
}

glm::mat4 Transform::GetRotationMatrix() {
	return rotationMatrix;
}

glm::mat4 Transform::GetLocalTransformationMatrix() {
	return transformationMatrix;
}

glm::mat4 Transform::GetTransformationMatrix() {
	glm::mat4 matrix(1.f);
	Transform* transform = this;
	do {
		matrix = transform->GetLocalTransformationMatrix() * matrix;
		transform = transform->parent;
	} while (transform != nullptr);
	return matrix;
}

glm::vec3 Transform::ToCylinder(glm::vec3 point) {
	float theta = point.x / radius;
	float r = radius - point.y;
	
	point.x = r * cos(theta);
	point.y = r * sin(theta);

	return point;
}

glm::vec3 Transform::FromCylinder(glm::vec3 point) {
	float r = sqrt(point.x*point.x + point.y*point.y);
	float theta = atan2(point.y, point.x);

	point.x = theta * radius;
	point.y = radius - r;

	return point;
}