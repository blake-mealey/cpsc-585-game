#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

const glm::vec3 Transform::FORWARD = glm::vec3(0, 0, -1);
const glm::vec3 Transform::RIGHT = glm::vec3(1, 0, 0);
const glm::vec3 Transform::UP = glm::vec3(0, 1, 0);

Transform::Transform() : Transform(nullptr, glm::vec3(), glm::vec3(1.f, 1.f, 1.f), glm::quat()) {}

Transform::Transform(Transform *pParent, glm::vec3 pPosition, glm::vec3 pScale, glm::quat pRotation) : parent(pParent) {
	SetPosition(pPosition);
	SetScale(pScale);
	SetRotation(pRotation);
}

glm::vec3 Transform::GetLocalPosition() {
	return position;
}

glm::vec3 Transform::GetLocalScale() {
	return scale;
}

glm::quat Transform::GetLocalRotation() {
	return rotation;
}

glm::vec3 Transform::GetGlobalPosition() {
	return GetTransformationMatrix() * glm::vec4(0.f, 0.f, 0.f, 1.f);
}


glm::vec3 Transform::GetForward() {
	return rotation * FORWARD;
}

glm::vec3 Transform::GetRight() {
	return rotation * RIGHT;
}

glm::vec3 Transform::GetUp() {
	return rotation * UP;
}


void Transform::UpdateTransformationMatrix() {
	transformationMatrix = translationMatrix * rotationMatrix * scalingMatrix;
}

void Transform::SetPosition(glm::vec3 pPosition) {
	position = pPosition;
	translationMatrix = glm::translate(glm::mat4(), position);
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
