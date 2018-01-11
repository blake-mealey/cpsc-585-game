#include "Transform.h"

using namespace glm;

Transform::Transform() : Transform(nullptr, vec3(), vec3(), quat()) {}

Transform::Transform(Transform *pParent, vec3 pPosition, vec3 pScale, quat pRotation) : parent(pParent) {
	SetPosition(position);
	SetScale(scale);
	SetRotation(rotation);
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
	transformationMatrix = scalingMatrix * translationMatrix * rotationMatrix;
}

void Transform::SetPosition(vec3 pPosition) {
	position = pPosition;
	translationMatrix = glm::translate(mat4(), position);
	UpdateTransformationMatrix();
}

void Transform::SetScale(vec3 pScale) {
	scale = pScale;
	scalingMatrix = glm::scale(mat4(), scale);
	UpdateTransformationMatrix();
}

void Transform::SetRotation(quat pRotation) {
	rotation = pRotation;
	rotationMatrix = glm::toMat4(rotation);
	UpdateTransformationMatrix();
}

void Transform::SetRotationEulerAngles(glm::vec3 eulerAngles) {
	SetRotation(quat(eulerAngles));
}

void Transform::SetRotationAxisAngles(glm::vec3 axis, float radians) {
	SetRotation(glm::angleAxis(degrees(radians), axis));
}


void Transform::Translate(glm::vec3 offset) {
	SetPosition(position + offset);
}

void Transform::Scale(float scaleFactor) {
	SetScale(scale * scaleFactor);
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
	mat4 matrix(1.f);
	Transform* transform = this;
	do {
		matrix = transform->GetLocalTransformationMatrix() * matrix;
		transform = transform->parent;
	} while (transform != nullptr);
	return matrix;
}
