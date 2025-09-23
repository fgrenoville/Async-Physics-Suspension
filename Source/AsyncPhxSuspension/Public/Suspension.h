// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SuspensionState.h"
#include "AsyncSuspensionIn.h"
#include "AsyncSuspensionOut.h"
#include "Components/SceneComponent.h"
#include "Suspension.generated.h"

UCLASS(ClassGroup = (AsyncPhxSuspension), meta = (BlueprintSpawnableComponent))
class ASYNCPHXSUSPENSION_API USuspension : public USceneComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AsyncPhxSuspension")
	float TravelCm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AsyncPhxSuspension")
	float SpringStiffness;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AsyncPhxSuspension")
	float ShockBump;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AsyncPhxSuspension")
	float ShockRebound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AsyncPhxSuspension")
	float MaxForce;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AsyncPhxSuspension")
	float WheelRadiusCm;

public:
	USuspension();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

	static void PhysicsTickComponent(UWorld* World, AActor* Vehicle,
		const FTransform AsyncVehicleTransformWorld, const FAsyncSuspensionIn& AsyncSuspensionIn,
		FAsyncSuspensionOut& AsyncSuspensionOut, FSuspensionState& SuspensionState, float PhysicsDeltaTime);

	static float GetTravelCmMinValue() { return TravelCmMinValue; }
	static float GetTravelCmMaxValue() { return TravelCmMaxValue; }
	static float GetTravelCmStepValue() { return TravelCmStepValue; }

	static float GetSpringStiffnessMinValue() { return SpringStiffnessMinValue; }
	static float GetSpringStiffnessMaxValue() { return SpringStiffnessMaxValue; }
	static float GetSpringStiffnessStepValue() { return SpringStiffnessStepValue; }
	
	static float GetShockBumpMinValue() { return ShockBumpMinValue; }
	static float GetShockBumpMaxValue() { return ShockBumpMaxValue; }
	static float GetShockBumpStepValue() { return ShockBumpStepValue; }

	static float GetShockReboundMinValue() { return ShockReboundMinValue; }
	static float GetShockReboundMaxValue() { return ShockReboundMaxValue; }
	static float GetShockReboundStepValue() { return ShockReboundStepValue; }

	static float GetMaxForceMinValue() { return MaxForceMinValue; }
	static float GetMaxForceMaxValue() { return MaxForceMaxValue; }
	static float GetMaxForceStepValue() { return MaxForceStepValue; }

	static float GetWheelRadiusCmMinValue() { return WheelRadiusCmMinValue; }
	static float GetWheelRadiusCmMaxValue() { return WheelRadiusCmMaxValue; }
	static float GetWheelRadiusCmStepValue() { return WheelRadiusCmStepValue; }

protected:
	virtual void BeginPlay() override;

private:
	static constexpr float ScaleDrawingForce = 0.02f;

	static constexpr float TravelCmMinValue = 10.0f;
	static constexpr float TravelCmMaxValue = 100.0f;
	static constexpr float TravelCmStepValue = 1.0f;

	static constexpr float SpringStiffnessMinValue = 10000.0f;
	static constexpr float SpringStiffnessMaxValue = 80000.0f;
	static constexpr float SpringStiffnessStepValue = 500.0f;

	static constexpr float ShockBumpMinValue = 100.0f;
	static constexpr float ShockBumpMaxValue = 10000.0f;
	static constexpr float ShockBumpStepValue = 100.0f;

	static constexpr float ShockReboundMinValue = 50.0f;
	static constexpr float ShockReboundMaxValue = 8000.0f;
	static constexpr float ShockReboundStepValue = 50.0f;

	static constexpr float MaxForceMinValue = 0.0f;
	static constexpr float MaxForceMaxValue = 12000.0f;
	static constexpr float MaxForceStepValue = 100.0f;

	static constexpr float WheelRadiusCmMinValue = 10.0f;
	static constexpr float WheelRadiusCmMaxValue = 60.0f;
	static constexpr float WheelRadiusCmStepValue = 1.0f;
};
