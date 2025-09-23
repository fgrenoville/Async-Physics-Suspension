// Fill out your copyright notice in the Description page of Project Settings.

#include "AsyncPhxSuspension/Public/AsyncCallback.h"
#include "Suspension.h"
#include "PhysicsProxy/SingleParticlePhysicsProxy.h"

void FAsyncCallback::OnPreSimulate_Internal()
{
	const FAsyncCallbackInput* In = GetConsumerInput_Internal();
	FAsyncCallbackOutput& Out = GetProducerOutputData_Internal();

	if (!In) return;

	const float PhysicsDeltaTime = GetDeltaTime_Internal();

	if (!In->PhxActorHandle) return;
		
	Chaos::FRigidBodyHandle_Internal* RigidHandle = In->PhxActorHandle->GetPhysicsThreadAPI();
	if (!RigidHandle) return;

	const FTransform VehicleTransformWorld = FTransform(RigidHandle->R(), RigidHandle->X());

	Out.FrameIndex = In->FrameIndex;
	
	USuspension::PhysicsTickComponent(In->World, In->Vehicle, VehicleTransformWorld, In->FrontLeftSuspension,
		Out.FrontLeftSuspension, FrontLeftSuspensionState, PhysicsDeltaTime);
	USuspension::PhysicsTickComponent(In->World, In->Vehicle, VehicleTransformWorld, In->FrontRightSuspension,
		Out.FrontRightSuspension, FrontRightSuspensionState, PhysicsDeltaTime);
	USuspension::PhysicsTickComponent(In->World, In->Vehicle, VehicleTransformWorld, In->RearLeftSuspension,
		Out.RearLeftSuspension, RearLeftSuspensionState, PhysicsDeltaTime);
	USuspension::PhysicsTickComponent(In->World, In->Vehicle, VehicleTransformWorld, In->RearRightSuspension,
		Out.RearRightSuspension, RearRightSuspensionState, PhysicsDeltaTime);
	
	AddForceAtLocation(RigidHandle, FrontLeftSuspensionState.ForceLocation, FrontLeftSuspensionState.Fz * 100.0f);
	AddForceAtLocation(RigidHandle, FrontRightSuspensionState.ForceLocation, FrontRightSuspensionState.Fz * 100.0f);
	AddForceAtLocation(RigidHandle, RearLeftSuspensionState.ForceLocation, RearLeftSuspensionState.Fz * 100.0f);
	AddForceAtLocation(RigidHandle, RearRightSuspensionState.ForceLocation, RearRightSuspensionState.Fz * 100.0f);
	
}

void FAsyncCallback::AddForceAtLocation(Chaos::FRigidBodyHandle_Internal* RigidHandle, const FVector Location, const FVector Force)
{
	if (!RigidHandle) return;

	const Chaos::FVec3 CoM = Chaos::FParticleUtilitiesGT::GetCoMWorldPosition(RigidHandle);
	const Chaos::FVec3 Torque = Chaos::FVec3::CrossProduct(Location - CoM, Force);
	RigidHandle->AddForce(Force, false);
	RigidHandle->AddTorque(Torque, false);

}