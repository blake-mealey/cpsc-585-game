#include "VehicleComponent.h"

#include "../Systems/Physics/VehicleCreate.h"

using namespace physx;

VehicleComponent::VehicleComponent() {
}

ComponentType VehicleComponent::GetType() {
    return ComponentType_Vehicle;
}

void VehicleComponent::HandleEvent(Event *event) {}