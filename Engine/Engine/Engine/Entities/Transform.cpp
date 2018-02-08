#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

const glm::vec3 Transform::FORWARD = glm::vec3(0, 0, -1);
const glm::vec3 Transform::RIGHT = glm::vec3(1, 0, 0);
const glm::vec3 Transform::UP = glm::vec3(0, 1, 0);

float Transform::radius = 0;

Transform::Transform() : Transform(nullptr, glm::vec3(), glm::vec3(1.f), glm::quat(), false) {}

Transform::Transform(physx::PxTransform t) : Transform(nullptr, FromPx(t.p), glm::vec3(1.f), FromPx(t.q), false) {}

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
	if (connectedToCylinder && radius > 0) {
		// bound to the location on the rectangle part of the cylinder
		while (position.x > M_PI * radius) {
			position.x -= 2 * M_PI * radius;
		}
		while (position.x < -M_PI * radius) {
			position.x += 4 * M_PI * radius;
		}
		translationMatrix = glm::translate(glm::mat4(), ToCylinder(position));
		//rotate accordingly
		float rotBy = position.x / radius + 90;
		rotationMatrix = glm::toMat4(glm::rotate(glm::quat(), rotBy, glm::vec3(0,0,1)))*glm::toMat4(rotation);
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
	if (connectedToCylinder && radius > 0) {
		rotationMatrix = glm::toMat4(glm::rotate(glm::quat(), position.x/radius, glm::vec3(0, 0, 1)))*glm::toMat4(rotation);
	}
	else {
		rotationMatrix = glm::toMat4(rotation);
		UpdateTransformationMatrix();
	}
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

// returns the world location of a point in the cylinder co-ordinates
// since the x coordinate will wrap transforming to the cylinder and then back may result in diffrent locations
glm::vec3 Transform::ToCylinder(glm::vec3 point) {
	float theta = point.x / radius;
	float r = radius - point.y;
	
	point.x = r * cos(theta);
	point.y = r * sin(theta);

	return point;
}

// returns the location on rectangular part of the unwrapped cylinder 
// x coordinates: [-radius*pi,radius*pi]
// y coordinates: [...,radius]
// z coordinates: [...,...]
glm::vec3 Transform::FromCylinder(glm::vec3 point) {
	float r = sqrt(point.x*point.x + point.y*point.y);
	float theta = atan2(point.y, point.x);

	point.x = theta * radius - M_PI * radius;
	point.y = radius - r;

	return point;
}



glm::vec4 Transform::FromPx(physx::PxVec4 v) {
	return glm::vec4(v.x, v.y, v.z, v.w);
}

glm::vec3 Transform::FromPx(physx::PxVec3 v) {
	return glm::vec3(v.x, v.y, v.z);
}

glm::vec2 Transform::FromPx(physx::PxVec2 v) {
	return glm::vec2(v.x, v.y);
}

glm::quat Transform::FromPx(physx::PxQuat q) {
	return glm::quat(q.w, q.x, q.y, q.z);
}
