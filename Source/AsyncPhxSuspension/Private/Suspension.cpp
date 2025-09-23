// Fill out your copyright notice in the Description page of Project Settings.

#include "Suspension.h"

USuspension::USuspension()
{
	PrimaryComponentTick.bCanEverTick = true;

	TravelCm = 80.0f;
	SpringStiffness = 35000.0f;
	ShockBump = 4300.0f;
	ShockRebound = 3000.0f;
	MaxForce = 7500.0f;
	
	WheelRadiusCm = 30.0f;
}

void USuspension::BeginPlay()
{
	Super::BeginPlay();
}

void USuspension::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USuspension::PhysicsTickComponent(UWorld* World, AActor* Vehicle, const FTransform AsyncVehicleTransformWorld,
	const FAsyncSuspensionIn& AsyncSuspensionIn, FAsyncSuspensionOut& AsyncSuspensionOut, FSuspensionState& SuspensionState, float PhysicsDeltaTime)
{
	const FTransform AsyncWorld = AsyncSuspensionIn.AttachLocal * AsyncVehicleTransformWorld;
	const FQuat AsyncRotation = AsyncWorld.GetRotation();

	FVector AsyncLocation = AsyncWorld.GetLocation();
	FVector AsyncUp = AsyncRotation.GetUpVector();
	
	FCollisionQueryParams TraceParams;
	TraceParams.MobilityType = EQueryMobilityType::Any;
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.AddIgnoredActor(Vehicle);
	TraceParams.bTraceComplex = true;

	SuspensionState.ForceLocation = AsyncLocation;
	
	FVector StartTrace = AsyncLocation;
	FVector EndTrace = StartTrace - AsyncUp * (AsyncSuspensionIn.TravelCm + AsyncSuspensionIn.WheelRadiusCm);

	AsyncSuspensionOut.AddDebugLineCommand(StartTrace, EndTrace, FColor::Magenta, 2.0f, 0.0f);

	FHitResult HitResult;

	if (bool bHasContact = World->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Visibility, TraceParams))
	{
		const FVector TraceVector = HitResult.ImpactPoint - StartTrace;
		const float HitDistanceInSuspensionAxis = FVector::DotProduct(TraceVector, -AsyncUp);

		const float CompressionRatio = FMath::Clamp(1.0f - (HitDistanceInSuspensionAxis / (AsyncSuspensionIn.TravelCm + AsyncSuspensionIn.WheelRadiusCm)), 0.0f, 1.0f);
		const float CompressionVelocity = (CompressionRatio - SuspensionState.PreviousCompressionRatio) / PhysicsDeltaTime;
		
		const float SpringForce = CompressionRatio * AsyncSuspensionIn.SpringStiffness;
		
		const float DampingForce = CompressionVelocity > 0.0f ?
			CompressionVelocity * AsyncSuspensionIn.ShockBump :				// Compression
				 CompressionVelocity * AsyncSuspensionIn.ShockRebound;		// Extension


		const float TotalForce = FMath::Clamp(SpringForce + DampingForce, 0.0f, AsyncSuspensionIn.MaxForce);
		const FVector Fz = TotalForce * AsyncUp;
		const FVector Fn = FMath::Max(0.0f, FVector::DotProduct(Fz, HitResult.ImpactNormal)) * HitResult.ImpactNormal;
		
		SuspensionState.PreviousCompressionRatio = CompressionRatio;
		SuspensionState.SpringForce = SpringForce;
		SuspensionState.DampingForce = DampingForce;
		SuspensionState.Fz = Fn;

		AsyncSuspensionOut.AddDebugLineCommand(SuspensionState.ForceLocation,
			SuspensionState.ForceLocation + SuspensionState.Fz * ScaleDrawingForce, FColor::Emerald,
			3.5f, 0.0f);
	}
	else
	{
		SuspensionState.PreviousCompressionRatio = 0.0f;
		SuspensionState.SpringForce = 0.0f;
		SuspensionState.DampingForce = 0.0f;
		SuspensionState.Fz = FVector::ZeroVector;
	}
}


