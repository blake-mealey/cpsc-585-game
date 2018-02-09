#pragma once

#include "System.h"

#include "PxPhysicsAPI.h"
#include "Physics/VehicleSceneQuery.h"
#include "Physics/VehicleCreate.h"

class Physics : public System {
public:
	// Access the singleton instance
	static Physics& Instance();
    ~Physics();

    void Initialize();

	void Update(Time currentTime, Time deltaTime) override;

private:
	// No instantiation or copying
	Physics();
	Physics(const Physics&) = delete;
	Physics& operator= (const Physics&) = delete;

    VehicleDesc initVehicleDesc();

    PxVehicleDrive4WRawInputData gVehicleInputData;
    static const PxVehiclePadSmoothingData gPadSmoothingData;
    static const PxF32 gSteerVsForwardSpeedData[2 * 8];
    static const PxFixedSizeLookupTable<8> gSteerVsForwardSpeedTable;

    physx::PxDefaultAllocator gAllocator;
    physx::PxDefaultErrorCallback gErrorCallback;

    physx::PxFoundation* gFoundation = NULL;
    physx::PxPhysics* gPhysics = NULL;
    
    physx::PxDefaultCpuDispatcher* gDispatcher = NULL;
    physx::PxScene* gScene = NULL;

    physx::PxCooking* gCooking = NULL;

    physx::PxMaterial* gMaterial = NULL;

    physx::PxPvd* gPvd = NULL;

    VehicleSceneQueryData* gVehicleSceneQueryData = NULL;
    physx::PxBatchQuery* gBatchQuery = NULL;

    physx::PxVehicleDrivableSurfaceToTireFrictionPairs* gFrictionPairs = NULL;

    physx::PxRigidStatic* gGroundPlane = NULL;
    //physx::PxVehicleDrive4W* gVehicle4W = NULL;

    bool gIsVehicleInAir = true;
};
