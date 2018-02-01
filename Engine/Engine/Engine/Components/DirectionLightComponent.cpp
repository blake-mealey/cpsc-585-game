#include "DirectionLightComponent.h"
#include "../Systems/Content/ContentManager.h"

DirectionLightComponent::DirectionLightComponent(nlohmann::json data) {
	color = ContentManager::JsonToVec3(data["Color"]);
	direction = ContentManager::JsonToVec3(data["Direction"]);
	castsShadows = ContentManager::GetFromJson<bool>(data["CastsShadows"], false);
}

DirectionLightComponent::DirectionLightComponent(glm::vec3 _color, glm::vec3 _direction)
	: color(_color), direction(_direction) {}

glm::vec3 DirectionLightComponent::GetColor() const {
	return color;
}

glm::vec3 DirectionLightComponent::GetDirection() const {
	return direction;
}

DirectionLight DirectionLightComponent::GetData() const {
	return DirectionLight(color, direction);
}

bool DirectionLightComponent::IsShadowCaster() const {
	return castsShadows;
}

void DirectionLightComponent::SetShadowCaster(bool _castsShadows) {
	castsShadows = _castsShadows;
}

ComponentType DirectionLightComponent::GetType() {
	return ComponentType_DirectionLight;
}

void DirectionLightComponent::HandleEvent(Event* event) {}
