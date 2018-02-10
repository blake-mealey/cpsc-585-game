#include "VehicleComponent.h"
#include "../Systems/Content/ContentManager.h"
#include "../Entities/EntityManager.h"

#include "../Systems/Physics/VehicleCreate.h"

using namespace physx;

VehicleComponent::VehicleComponent() : VehicleComponent(4, false) { }

VehicleComponent::VehicleComponent(nlohmann::json data) {
	wheelCount = ContentManager::GetFromJson<size_t>(data["WheelCount"], 4);
	inputTypeDigital = ContentManager::GetFromJson<bool>(data["DigitalInput"], false);

	Initialize();
}

VehicleComponent::VehicleComponent(size_t _wheelCount, bool _inputTypeDigital) : wheelCount(_wheelCount), inputTypeDigital(_inputTypeDigital) {
	Initialize();
}

void VehicleComponent::Initialize() {
	for (size_t i = 0; i < wheelCount; ++i) {
		MeshComponent* wheel = new MeshComponent("Boulder.obj", "Basic.json", "Boulder.jpg");
		wheel->transform.SetScale(glm::vec3(0.5f));
		wheelMeshes.push_back(wheel);
	}
}

void VehicleComponent::UpdateWheelTransforms() {
	PxShape** shapes = new PxShape*[wheelCount];
	pxVehicle->getRigidDynamicActor()->getShapes(shapes, wheelCount, 0);
	for (size_t i = 0; i < wheelCount; ++i) {
		MeshComponent* wheel = wheelMeshes[i];
		PxTransform pose = shapes[i]->getLocalPose();
		wheel->transform.SetPosition(Transform::FromPx(pose.p));
		wheel->transform.SetRotation(Transform::FromPx(pose.q));
		//wheel->transform.Rotate(Transform::UP, glm::radians(90.f));
	}
	delete[] shapes;
}

ComponentType VehicleComponent::GetType() {
    return ComponentType_Vehicle;
}

void VehicleComponent::HandleEvent(Event *event) {}

void VehicleComponent::SetEntity(Entity* _entity) {
	Component::SetEntity(_entity);
	for (MeshComponent *component : wheelMeshes) {
		/*if (component->GetEntity() != nullptr) {
			EntityManager::RemoveComponent(component->GetEntity(), component);
		}*/
		EntityManager::AddComponent(GetEntity(), component);
	}
}