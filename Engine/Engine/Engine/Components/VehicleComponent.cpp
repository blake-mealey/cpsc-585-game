#include "VehicleComponent.h"
#include "../Systems/Content/ContentManager.h"
#include "../Entities/EntityManager.h"

#include "../Systems/Physics/VehicleCreate.h"

using namespace physx;

VehicleComponent::VehicleComponent() : VehicleComponent(4, false) { }

VehicleComponent::VehicleComponent(nlohmann::json data) {
	inputTypeDigital = ContentManager::GetFromJson<bool>(data["DigitalInput"], false);

    wheelMeshPath = ContentManager::GetFromJson<std::string>(data["WheelMesh"], "Boulder.obj");
    wheelTexturePath = ContentManager::GetFromJson<std::string>(data["WheelTexture"], "Boulder.jpg");
    wheelMaterialPath = ContentManager::GetFromJson<std::string>(data["WheelMaterial"], "Basic.json");

    chassisMass = ContentManager::GetFromJson<float>(data["ChassisMass"], 1500.f);
    chassisSize = ContentManager::JsonToVec3(data["ChassisSize"], glm::vec3(2.5f, 2.f, 5.f));

    wheelMass = ContentManager::GetFromJson<float>(data["WheelMass"], 20.f);
    wheelRadius = ContentManager::GetFromJson<float>(data["WheelRadius"], 0.5f);
    wheelWidth = ContentManager::GetFromJson<float>(data["WheelWidth"], 0.4f);
    wheelCount = ContentManager::GetFromJson<size_t>(data["WheelCount"], 4);

	Initialize();
}

VehicleComponent::VehicleComponent(size_t _wheelCount, bool _inputTypeDigital) :
        inputTypeDigital(_inputTypeDigital), wheelMeshPath("Boulder.obj"), wheelTexturePath("Boulder.jpg"), wheelMaterialPath("Basic.json"),
        chassisMass(1500.f), chassisSize(glm::vec3(2.5f, 2.f, 5.f)), wheelMass(20.f), wheelRadius(0.5f), wheelWidth(0.4f), wheelCount(_wheelCount) {
	
    Initialize();
}

void VehicleComponent::Initialize() {
	for (size_t i = 0; i < wheelCount; ++i) {
		MeshComponent* wheel = new MeshComponent(wheelMeshPath, wheelMaterialPath, wheelTexturePath);
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

float VehicleComponent::GetChassisMass() const {
    return chassisMass;
}

glm::vec3 VehicleComponent::GetChassisSize() const {
    return chassisSize;
}

glm::vec3 VehicleComponent::GetChassisMomentOfInertia() const {
    return glm::vec3((chassisSize.y*chassisSize.y + chassisSize.z*chassisSize.z)*chassisMass / 12.0f,
        (chassisSize.x*chassisSize.x + chassisSize.z*chassisSize.z)*0.8f*chassisMass / 12.0f,
        (chassisSize.x*chassisSize.x + chassisSize.y*chassisSize.y)*chassisMass / 12.0f);
}

glm::vec3 VehicleComponent::GetChassisCenterOfMassOffset() const {
    return glm::vec3(0.0f, -chassisSize.y*0.5f + 0.65f, 0.25f);
}

float VehicleComponent::GetWheelMass() const {
    return wheelMass;
}

float VehicleComponent::GetWheelRadius() const {
    return wheelRadius;
}

float VehicleComponent::GetWheelWidth() const {
    return wheelWidth;
}

float VehicleComponent::GetWheelMomentOfIntertia() const {
    return 0.5f*wheelMass*wheelRadius*wheelRadius;
}

size_t VehicleComponent::GetWheelCount() const {
    return wheelCount;
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