#include "CameraComponent.h"
#include <glm/gtc/matrix_transform.inl>

const float CameraComponent::NEAR_CLIPPING_PLANE = 0.1f;
const float CameraComponent::FAR_CLIPPING_PLANE = 100.f;
const float CameraComponent::DEFAULT_FIELD_OF_VIEW = 60.f;		// In degrees

ComponentType CameraComponent::GetType() {
	return ComponentType_CameraComponent;
}

void CameraComponent::HandleEvent(Event* event) {}

CameraComponent::CameraComponent() : CameraComponent(glm::vec3(0, 0, -10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)) {}

CameraComponent::CameraComponent(glm::vec3 _position, glm::vec3 _target, glm::vec3 _upVector) :
	position(_position), target(_target), upVector(_upVector), fieldOfView(DEFAULT_FIELD_OF_VIEW) {
	
	UpdateViewMatrix();
}

glm::vec3 CameraComponent::GetPosition() const {
	return position;
}

glm::vec3 CameraComponent::GetTarget() const {
	return target;
}

float CameraComponent::GetFieldOfView() const {
	return fieldOfView;
}

void CameraComponent::SetPosition(const glm::vec3 _position) {
	position = _position;
	UpdateViewMatrix();
}

void CameraComponent::SetTarget(const glm::vec3 _target) {
	target = _target;
	UpdateViewMatrix();
}

void CameraComponent::SetFieldOfView(const float _fieldOfView) {
	fieldOfView = _fieldOfView;
	UpdateProjectionMatrix();
}

void CameraComponent::SetAspectRatio(const float _aspectRatio) {
	aspectRatio = _aspectRatio;
	UpdateProjectionMatrix();
}

glm::mat4 CameraComponent::GetViewMatrix() const {
	return viewMatrix;
}

glm::mat4 CameraComponent::GetProjectionMatrix() const {
	return projectionMatrix;
}

void CameraComponent::UpdateViewMatrix() {
	viewMatrix = glm::lookAt(position, target, upVector);
}

void CameraComponent::UpdateProjectionMatrix() {
	projectionMatrix = glm::perspective(glm::radians(fieldOfView), aspectRatio, NEAR_CLIPPING_PLANE, FAR_CLIPPING_PLANE);
}
