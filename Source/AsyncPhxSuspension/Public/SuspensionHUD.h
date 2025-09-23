// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SuspensionHUD.generated.h"

class USuspension;
class UWidgetComponent;
class USuspensionWidget;

UCLASS(ClassGroup = (AsyncPhxSuspension))
class ASYNCPHXSUSPENSION_API USuspensionHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AsyncPhxSuspension|HUD")
	TSubclassOf<USuspensionWidget> SuspensionWidgetClass;

public:
	void Setup(USuspension* FrontLeftSuspension, USuspension* FrontRightSuspension,
		USuspension* RearLeftSuspension, USuspension* RearRightSuspension);
	
protected:
	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* Grid;

private:
	UPROPERTY()
	TObjectPtr<USuspensionWidget> FrontLeftSuspensionWidget;
	UPROPERTY()
	TObjectPtr<USuspensionWidget> FrontRightSuspensionWidget;
	UPROPERTY()
	TObjectPtr<USuspensionWidget> RearLeftSuspensionWidget;
	UPROPERTY()
	TObjectPtr<USuspensionWidget> RearRightSuspensionWidget;

private:
	USuspensionWidget* AddWidget(USuspension* Component, const FText& Label, int32 Row, int32 Col);
};
