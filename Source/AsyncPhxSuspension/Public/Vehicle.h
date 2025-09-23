// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AsyncSuspensionOut.h"
#include "AsyncSuspensionIn.h"
#include "Vehicle.generated.h"

class USuspensionHUD;
class UWidgetComponent;
class UCameraComponent;
class USpringArmComponent;
class USuspension;
class FAsyncCallback;

UCLASS(ClassGroup = (AsyncPhxSuspension))
class ASYNCPHXSUSPENSION_API AVehicle : public APawn
{
	GENERATED_BODY()

public:

public:
	AVehicle();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "AsyncPhxSuspension")
	void ToggleBodyMeshVisibility();
	UFUNCTION(BlueprintCallable, Category = "AsyncPhxSuspension")
	void ToggleDebugVisibility();
	
	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	FAsyncCallback* AsyncCallback;

	UPROPERTY()
	TWeakObjectPtr<UWorld> CachedWorld;
	
	UPROPERTY(EditAnywhere, Category = "AsyncPhxSuspension")
	TObjectPtr<UStaticMeshComponent> BodyMeshComponent;	
	
	UPROPERTY(EditAnywhere, Category = "AsyncPhxSuspension")
	TObjectPtr<USuspension> FrontLeftSuspension;
	UPROPERTY(EditAnywhere, Category = "AsyncPhxSuspension")
	TObjectPtr<USuspension> FrontRightSuspension;
	UPROPERTY(EditAnywhere, Category = "AsyncPhxSuspension")
	TObjectPtr<USuspension> RearLeftSuspension;
	UPROPERTY(EditAnywhere, Category = "AsyncPhxSuspension")
	TObjectPtr<USuspension> RearRightSuspension;

	UPROPERTY(EditAnywhere, Category = "AsyncPhxSuspension")
	TObjectPtr<USpringArmComponent> SpringArm;
	UPROPERTY(EditAnywhere, Category = "AsyncPhxSuspension")
	TObjectPtr<UCameraComponent> Camera;
	
	UPROPERTY(EditAnywhere, Category = "AsyncPhxSuspension")
	TSubclassOf<USuspensionHUD> SuspensionHUDClass;
	UPROPERTY()
	TObjectPtr<USuspensionHUD> SuspensionHUD;
	
	bool bDebugVisibility;
	
private:
	void HandlePrePhysicsTick(FPhysScene* PhysScene, float DeltaSeconds);
	static void BuildAsyncInputStruct(FAsyncSuspensionIn& AsyncInput, const USuspension* Suspension);
	static void DrawSuspensionDebug(const UWorld* World, const FAsyncSuspensionOut& Suspension);
};
