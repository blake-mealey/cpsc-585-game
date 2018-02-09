#include "VehicleComponent.h"

#include "../Systems/Physics/VehicleCreate.h"

using namespace physx;

VehicleComponent::VehicleComponent() : inputTypeDigital(false) {
}

ComponentType VehicleComponent::GetType() {
    return ComponentType_Vehicle;
}

void VehicleComponent::HandleEvent(Event *event) {}