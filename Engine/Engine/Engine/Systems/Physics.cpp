#include "Physics.h"

#include "PxPhysicsAPI.h"

#include "vehicle/PxVehicleUtil.h"

#include "../Entities/Entity.h"
#include "../Entities/EntityManager.h"
#include "../Entities/Transform.h"

#include <iostream>
#include "Physics/VehicleSceneQuery.h"
#include "Physics/VehicleFilterShader.h"
#include "Physics/VehicleTireFriction.h"
#include "Physics/VehicleCreate.h"
#include "Game.h"
#include "../Components/VehicleComponent.h"
using namespace std;

using namespace physx;

float nextTime = 0;
int i = 1;

// Singleton
Physics::Physics() { }
Physics &Physics::Instance() {
	static Physics instance;
	return instance;
}

Physics::~Physics() {
    /*gVehicle4W->getRigidDynamicActor()->release();        // TODO: VehicleComponent destructor
    gVehicle4W->free();*/
    gGroundPlane->release();
    gBatchQuery->release();
    gVehicleSceneQueryData->free(gAllocator);
    gFrictionPairs->release();
    PxCloseVehicleSDK();

    gMaterial->release();
    gCooking->release();
    gScene->release();
    gDispatcher->release();
    gPhysics->release();
    PxPvdTransport* transport = gPvd->getTransport();
    gPvd->release();
    transport->release();
    gFoundation->release();
}

const PxVehiclePadSmoothingData Physics::gPadSmoothingData =
{
    {
        6.0f,	//rise rate eANALOG_INPUT_ACCEL
        6.0f,	//rise rate eANALOG_INPUT_BRAKE		
        6.0f,	//rise rate eANALOG_INPUT_HANDBRAKE	
        2.5f,	//rise rate eANALOG_INPUT_STEER_LEFT
        2.5f,	//rise rate eANALOG_INPUT_STEER_RIGHT
    },
    {
        10.0f,	//fall rate eANALOG_INPUT_ACCEL
        10.0f,	//fall rate eANALOG_INPUT_BRAKE		
        10.0f,	//fall rate eANALOG_INPUT_HANDBRAKE	
        5.0f,	//fall rate eANALOG_INPUT_STEER_LEFT
        5.0f	//fall rate eANALOG_INPUT_STEER_RIGHT
    }
};

const PxF32 Physics::gSteerVsForwardSpeedData[2 * 8] =
{
    0.0f,		0.75f,
    5.0f,		0.75f,
    30.0f,		0.125f,
    120.0f,		0.1f,
    PX_MAX_F32, PX_MAX_F32,
    PX_MAX_F32, PX_MAX_F32,
    PX_MAX_F32, PX_MAX_F32,
    PX_MAX_F32, PX_MAX_F32
};
const PxFixedSizeLookupTable<8> Physics::gSteerVsForwardSpeedTable(gSteerVsForwardSpeedData, 4);

void Physics::Initialize() {
    gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);
    gPvd = PxCreatePvd(*gFoundation);
    PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
    gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
    gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);

    PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);

    PxU32 numWorkers = 1;
    gDispatcher = PxDefaultCpuDispatcherCreate(numWorkers);
    sceneDesc.cpuDispatcher = gDispatcher;
    sceneDesc.filterShader = VehicleFilterShader;

    gScene = gPhysics->createScene(sceneDesc);
    PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
    if (pvdClient)
    {
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
    }
    gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

    gCooking = PxCreateCooking(PX_PHYSICS_VERSION, *gFoundation, PxCookingParams(PxTolerancesScale()));

    /////////////////////////////////////////////

    PxInitVehicleSDK(*gPhysics);
    PxVehicleSetBasisVectors(PxVec3(0, 1, 0), PxVec3(0, 0, 1));
    PxVehicleSetUpdateMode(PxVehicleUpdateMode::eVELOCITY_CHANGE);

    vector<Component*> vehicleComponents = EntityManager::GetComponents(ComponentType_Vehicle);

    //Create the batched scene queries for the suspension raycasts.
    const size_t vehicleCount = vehicleComponents.size();
    gVehicleSceneQueryData = VehicleSceneQueryData::allocate(Game::MAX_VEHICLE_COUNT, PX_MAX_NB_WHEELS, 1, vehicleCount, WheelSceneQueryPreFilterBlocking, NULL, gAllocator);
    gBatchQuery = VehicleSceneQueryData::setUpBatchedSceneQuery(0, *gVehicleSceneQueryData, gScene);

    //Create the friction table for each combination of tire and surface type.
    gFrictionPairs = createFrictionPairs(gMaterial);

    //Create a plane to drive on.
    PxFilterData groundPlaneSimFilterData(COLLISION_FLAG_GROUND, COLLISION_FLAG_GROUND_AGAINST, 0, 0);
    gGroundPlane = createDrivablePlane(groundPlaneSimFilterData, gMaterial, gPhysics);
    gScene->addActor(*gGroundPlane);


    for (Component* component : vehicleComponents) {
        VehicleComponent* vehicle = static_cast<VehicleComponent*>(component);

        //Create a vehicle that will drive on the plane.
        VehicleDesc vehicleDesc = initVehicleDesc();
        vehicle->pxVehicle = createVehicle4W(vehicleDesc, gPhysics, gCooking);
        
        //PxTransform startTransform(PxVec3(0, (vehicleDesc.chassisDims.y*0.5f + vehicleDesc.wheelRadius + 1.0f), 0), PxQuat(PxIdentity));
        vehicle->pxVehicle->getRigidDynamicActor()->setGlobalPose(Transform::ToPx(component->GetEntity()->transform));
        gScene->addActor(*vehicle->pxVehicle->getRigidDynamicActor());

        //Set the vehicle to rest in first gear.
        //Set the vehicle to use auto-gears.
        vehicle->pxVehicle->setToRestState();
        vehicle->pxVehicle->mDriveDynData.forceGearChange(PxVehicleGearsData::eFIRST);
        vehicle->pxVehicle->mDriveDynData.setUseAutoGears(true);
    }
}

VehicleDesc Physics::initVehicleDesc()
{
    //Set up the chassis mass, dimensions, moment of inertia, and center of mass offset.
    //The moment of inertia is just the moment of inertia of a cuboid but modified for easier steering.
    //Center of mass offset is 0.65m above the base of the chassis and 0.25m towards the front.
    const PxF32 chassisMass = 1500.0f;
    const PxVec3 chassisDims(2.5f, 2.0f, 5.0f);
    const PxVec3 chassisMOI
    ((chassisDims.y*chassisDims.y + chassisDims.z*chassisDims.z)*chassisMass / 12.0f,
        (chassisDims.x*chassisDims.x + chassisDims.z*chassisDims.z)*0.8f*chassisMass / 12.0f,
        (chassisDims.x*chassisDims.x + chassisDims.y*chassisDims.y)*chassisMass / 12.0f);
    const PxVec3 chassisCMOffset(0.0f, -chassisDims.y*0.5f + 0.65f, 0.25f);

    //Set up the wheel mass, radius, width, moment of inertia, and number of wheels.
    //Moment of inertia is just the moment of inertia of a cylinder.
    const PxF32 wheelMass = 20.0f;
    const PxF32 wheelRadius = 0.5f;
    const PxF32 wheelWidth = 0.4f;
    const PxF32 wheelMOI = 0.5f*wheelMass*wheelRadius*wheelRadius;
    const PxU32 nbWheels = 6;

    VehicleDesc vehicleDesc;

    vehicleDesc.chassisMass = chassisMass;
    vehicleDesc.chassisDims = chassisDims;
    vehicleDesc.chassisMOI = chassisMOI;
    vehicleDesc.chassisCMOffset = chassisCMOffset;
    vehicleDesc.chassisMaterial = gMaterial;
    vehicleDesc.chassisSimFilterData = PxFilterData(COLLISION_FLAG_CHASSIS, COLLISION_FLAG_CHASSIS_AGAINST, 0, 0);      // ?????????

    vehicleDesc.wheelMass = wheelMass;
    vehicleDesc.wheelRadius = wheelRadius;
    vehicleDesc.wheelWidth = wheelWidth;
    vehicleDesc.wheelMOI = wheelMOI;
    vehicleDesc.numWheels = nbWheels;
    vehicleDesc.wheelMaterial = gMaterial;
    vehicleDesc.chassisSimFilterData = PxFilterData(COLLISION_FLAG_WHEEL, COLLISION_FLAG_WHEEL_AGAINST, 0, 0);

    return vehicleDesc;
}

void Physics::Update(Time currentTime, Time deltaTime) {
    const PxF32 timestep = 1.0f / 60.0f;

    /*
    gVehicleInputData.setAnalogAccel(1.f);

    //Update the control inputs for the vehicle.
    PxVehicleDrive4WSmoothAnalogRawInputsAndSetAnalogInputs(gPadSmoothingData, gSteerVsForwardSpeedTable, gVehicleInputData, timestep, gIsVehicleInAir, *gVehicle4W);*/

    //Raycasts.
    vector<Component*> vehicleComponents = EntityManager::GetComponents(ComponentType_Vehicle);
    vector<PxVehicleWheels*> vehicles;
    for (Component* component : vehicleComponents) {
        VehicleComponent* vehicle = static_cast<VehicleComponent*>(component);
        vehicles.push_back(vehicle->pxVehicle);
    }

    PxRaycastQueryResult* raycastResults = gVehicleSceneQueryData->getRaycastQueryResultBuffer(0);
    const PxU32 raycastResultsSize = gVehicleSceneQueryData->getQueryResultBufferSize();
    PxVehicleSuspensionRaycasts(gBatchQuery, vehicles.size(), vehicles.data(), raycastResultsSize, raycastResults);
    
    //Vehicle update.
    const PxVec3 grav = gScene->getGravity();
    PxWheelQueryResult wheelQueryResults[PX_MAX_NB_WHEELS];
    vector<PxVehicleWheelQueryResult> vehicleQueryResults;
    for (PxVehicleWheels* vehicle : vehicles) {
        vehicleQueryResults.push_back({ wheelQueryResults, vehicle->mWheelsSimData.getNbWheels() });
    }
    PxVehicleUpdates(timestep, grav, *gFrictionPairs, vehicles.size(), vehicles.data(), vehicleQueryResults.data());

    //Work out if the vehicle is in the air.
    for (PxVehicleWheels* vehicle : vehicles) {
        gIsVehicleInAir = vehicle->getRigidDynamicActor()->isSleeping() ? false : PxVehicleIsInAir(vehicleQueryResults[0]);
    }

    //Scene update.
    gScene->simulate(timestep);
    gScene->fetchResults(true);


	/*if (currentTime.GetTimeSeconds() >= nextTime) {
		gVehicle4W->getRigidDynamicActor()->addForce(PxVec3(500000.0f * i, 1000000.0f, 0.0f), PxForceMode::eFORCE, true);
		nextTime = currentTime.GetTimeSeconds() + 5.0f;
		i *= -1;
	}*/

    for (Component* component : vehicleComponents) {
        VehicleComponent* vehicle = static_cast<VehicleComponent*>(component);
        PxTransform t = vehicle->pxVehicle->getRigidDynamicActor()->getGlobalPose();
        component->GetEntity()->transform = Transform(t);
    }
}