#pragma once

#include "Component.h"
#include <vehicle/PxVehicleDrive4W.h>
#include <vehicle/PxVehicleUtilControl.h>

class VehicleComponent : public Component {
public:
    VehicleComponent();

    ComponentType GetType();
    void HandleEvent(Event *event);
    physx::PxVehicleDrive4W* pxVehicle = nullptr;
	physx::PxVehicleDrive4WRawInputData pxVehicleInputData;

private:
};
