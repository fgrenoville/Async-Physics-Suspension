// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SuspensionState.h"
#include "AsyncSuspensionIn.h"
#include "AsyncSuspensionOut.h"

struct ASYNCPHXSUSPENSION_API FAsyncCallbackInput : public Chaos::FSimCallbackInput
{
	UWorld* World = nullptr;
	AActor* Vehicle = nullptr;
		
	int32 FrameIndex = 0;
	
	FPhysicsActorHandle PhxActorHandle;

	FAsyncSuspensionIn FrontLeftSuspension;
	FAsyncSuspensionIn FrontRightSuspension;
	FAsyncSuspensionIn RearLeftSuspension;
	FAsyncSuspensionIn RearRightSuspension;

	void Reset()
	{
		*this = FAsyncCallbackInput();
	}
};

struct ASYNCPHXSUSPENSION_API FAsyncCallbackOutput : public Chaos::FSimCallbackOutput
{
	int32 FrameIndex = 0;

	FAsyncSuspensionOut FrontLeftSuspension;
	FAsyncSuspensionOut FrontRightSuspension;
	FAsyncSuspensionOut RearLeftSuspension;
	FAsyncSuspensionOut RearRightSuspension;
	
	void Reset()
	{
		*this = FAsyncCallbackOutput();
	}
};

class ASYNCPHXSUSPENSION_API FAsyncCallback : public Chaos::TSimCallbackObject<FAsyncCallbackInput, FAsyncCallbackOutput>
{
private:
	FSuspensionState FrontLeftSuspensionState;
	FSuspensionState FrontRightSuspensionState;
	FSuspensionState RearLeftSuspensionState;
	FSuspensionState RearRightSuspensionState;
	
private:
	virtual void OnPreSimulate_Internal() override;
	static void AddForceAtLocation(Chaos::FRigidBodyHandle_Internal* RigidHandle, const FVector Location, const FVector Force);
};
