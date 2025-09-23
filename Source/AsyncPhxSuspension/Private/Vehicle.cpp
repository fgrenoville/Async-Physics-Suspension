// Fill out your copyright notice in the Description page of Project Settings.

#include "AsyncPhxSuspension/Public/Vehicle.h"
#include "PBDRigidsSolver.h"
#include "SuspensionHUD.h"
#include "AsyncPhxSuspension/Public/AsyncCallback.h"
#include "AsyncPhxSuspension/Public/Suspension.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Physics/Experimental/PhysScene_Chaos.h"

AVehicle::AVehicle()
{
	PrimaryActorTick.bCanEverTick = true;
	
	AsyncCallback = nullptr;
	CachedWorld = nullptr;
	
	BodyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	RootComponent = BodyMeshComponent;
	
	FrontLeftSuspension = CreateDefaultSubobject<USuspension>("FrontLeftSuspension");
	FrontLeftSuspension->SetupAttachment(BodyMeshComponent);

	FrontRightSuspension = CreateDefaultSubobject<USuspension>("FrontRightSuspension");
	FrontRightSuspension->SetupAttachment(BodyMeshComponent);

	RearLeftSuspension = CreateDefaultSubobject<USuspension>("RearLeftSuspension");
	RearLeftSuspension->SetupAttachment(BodyMeshComponent);

	RearRightSuspension = CreateDefaultSubobject<USuspension>("RearRightSuspension");
	RearRightSuspension->SetupAttachment(BodyMeshComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(BodyMeshComponent);
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	SuspensionHUDClass = nullptr;
	SuspensionHUD = nullptr;
	
	bDebugVisibility = false;
}

void AVehicle::BeginPlay()
{
	Super::BeginPlay();

	CachedWorld = GetWorld();
	if (!CachedWorld.IsValid()) return;

	if (!IsValid(FrontLeftSuspension) || !IsValid(FrontRightSuspension) ||
		!IsValid(RearLeftSuspension) || !IsValid(RearRightSuspension))
		return;
	
	if (FPhysScene* PhysScene = CachedWorld->GetPhysicsScene())
	{
		if (!AsyncCallback)
		{
			AsyncCallback = PhysScene->GetSolver()->CreateAndRegisterSimCallbackObject_External<FAsyncCallback>();
			PhysScene->OnPhysScenePreTick.AddUObject(this, &AVehicle::HandlePrePhysicsTick);
		}
	}

	BodyMeshComponent->SetMassOverrideInKg(NAME_None, 1500.0f);
	BodyMeshComponent->SetSimulatePhysics(true);
	BodyMeshComponent->SetEnableGravity(true);

	if (APlayerController* PlayerController = CachedWorld->GetFirstPlayerController())
	{
		if (IsValid(SuspensionHUDClass))
		{
			SuspensionHUD = CreateWidget<USuspensionHUD>(PlayerController, SuspensionHUDClass);
			if (SuspensionHUD)
			{
				SuspensionHUD->AddToViewport();
				SuspensionHUD->Setup(FrontLeftSuspension, FrontRightSuspension, RearLeftSuspension, RearRightSuspension);

				PlayerController->bShowMouseCursor = true;
				
			}
		}
	}
}

void AVehicle::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (!CachedWorld.IsValid()) return;

	if (FPhysScene* PhysScene = CachedWorld->GetPhysicsScene())
	{
		if (AsyncCallback)
		{
			PhysScene->GetSolver()->UnregisterAndFreeSimCallbackObject_External(AsyncCallback);
			AsyncCallback = nullptr;
		}
		
		PhysScene->OnPhysScenePreTick.RemoveAll(this);
	}
	
	Super::EndPlay(EndPlayReason);
}


void AVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!AsyncCallback) return;
	if (!CachedWorld.IsValid()) return;

	if (!IsValid(FrontLeftSuspension) || !IsValid(FrontRightSuspension) ||
			!IsValid(RearLeftSuspension) || !IsValid(RearRightSuspension))
		return;

	BodyMeshComponent->WakeRigidBody();
	
	while (Chaos::TSimCallbackOutputHandle<FAsyncCallbackOutput> OutH = AsyncCallback->PopOutputData_External())
	{
		const FAsyncCallbackOutput* Out =  OutH.Get();
		if (!Out) continue;
		
		if (bDebugVisibility)
		{
			DrawSuspensionDebug(CachedWorld.Get(), Out->FrontLeftSuspension);
			DrawSuspensionDebug(CachedWorld.Get(), Out->FrontRightSuspension);
			DrawSuspensionDebug(CachedWorld.Get(), Out->RearLeftSuspension);
			DrawSuspensionDebug(CachedWorld.Get(), Out->RearRightSuspension);
		}
	}
}

void AVehicle::ToggleBodyMeshVisibility()
{
	if (!IsValid(BodyMeshComponent)) return;
	
	BodyMeshComponent->SetHiddenInGame(!BodyMeshComponent->bHiddenInGame);
}

void AVehicle::ToggleDebugVisibility()
{
	bDebugVisibility = !bDebugVisibility;
}

void AVehicle::HandlePrePhysicsTick(FPhysScene* PhysScene, float DeltaSeconds)
{
	if (!AsyncCallback) return;
	if (!CachedWorld.IsValid()) return;
	
	FAsyncCallbackInput* AsyncInput = AsyncCallback->GetProducerInputData_External();
	if (!AsyncInput) return;

	const FBodyInstance* PhxBodyInstance = BodyMeshComponent->GetBodyInstance();
	if (!PhxBodyInstance) return;

	if (!IsValid(FrontLeftSuspension) || !IsValid(FrontRightSuspension) ||
	!IsValid(RearLeftSuspension) || !IsValid(RearRightSuspension))
		return;
	
	AsyncInput->World = CachedWorld.Get();
	
	AsyncInput->Vehicle = this;
		
	AsyncInput->FrameIndex++;

	AsyncInput->PhxActorHandle = PhxBodyInstance->GetPhysicsActorHandle();

	BuildAsyncInputStruct(AsyncInput->FrontLeftSuspension, FrontLeftSuspension);
	BuildAsyncInputStruct(AsyncInput->FrontRightSuspension, FrontRightSuspension);
	BuildAsyncInputStruct(AsyncInput->RearLeftSuspension, RearLeftSuspension);
	BuildAsyncInputStruct(AsyncInput->RearRightSuspension, RearRightSuspension);
}

void AVehicle::BuildAsyncInputStruct(FAsyncSuspensionIn& AsyncInput, const USuspension* Suspension)
{
	AsyncInput.AttachLocal = Suspension->GetRelativeTransform();
	AsyncInput.TravelCm = Suspension->TravelCm;
	AsyncInput.SpringStiffness = Suspension->SpringStiffness;
	AsyncInput.ShockBump = Suspension->ShockBump;
	AsyncInput.ShockRebound = Suspension->ShockRebound;
	AsyncInput.MaxForce = Suspension->MaxForce;
	AsyncInput.WheelRadiusCm = Suspension->WheelRadiusCm;
}

void AVehicle::DrawSuspensionDebug(const UWorld* World, const FAsyncSuspensionOut& Suspension)
{
	for (const FDebugDrawLineCommand& Command : Suspension.DebugDrawCommands)
	{
		DrawDebugLine(World,
			Command.StartPoint,
			Command.EndPoint,
			Command.Color,
			false, 
			Command.Duration,
			0,
			Command.Thickness);
	}
}

// void AVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
// {
// 	Super::SetupPlayerInputComponent(PlayerInputComponent);
// }

