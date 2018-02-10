#pragma once

#include <vector>

#include "Component.h"
#include "MeshComponent.h"
#include <vehicle/PxVehicleDrive4W.h>
#include <vehicle/PxVehicleUtilControl.h>
#include <json/json.hpp>

class VehicleComponent : public Component {
public:
	VehicleComponent(nlohmann::json data);
	VehicleComponent(size_t _wheelCount, bool _inputTypeDigital);
    VehicleComponent();

    ComponentType GetType();
    void HandleEvent(Event *event);
    physx::PxVehicleDrive4W* pxVehicle = nullptr;
	physx::PxVehicleDrive4WRawInputData pxVehicleInputData;
    bool inputTypeDigital;

	void SetEntity(Entity* _entity) override;

	void UpdateWheelTransforms();

private:

	size_t wheelCount;
	std::vector<MeshComponent*> wheelMeshes;

	void Initialize();
};
