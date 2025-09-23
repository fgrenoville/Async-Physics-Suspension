// Fill out your copyright notice in the Description page of Project Settings.

#include "SuspensionHUD.h"
#include "Suspension.h"
#include "SuspensionWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"

void USuspensionHUD::Setup(USuspension* FrontLeftSuspension, USuspension* FrontRightSuspension,
                           USuspension* RearLeftSuspension, USuspension* RearRightSuspension)
{
	if (!Grid) return;
	Grid->ClearChildren();
	
	FrontLeftSuspensionWidget = AddWidget(FrontLeftSuspension, FText::FromString(TEXT("Front Left Suspension")), 0, 0);
	FrontRightSuspensionWidget = AddWidget(FrontRightSuspension, FText::FromString(TEXT("Front Right Suspension")), 0, 1);
	RearLeftSuspensionWidget = AddWidget(RearLeftSuspension, FText::FromString(TEXT("Rear Left Suspension")), 1, 0);
	RearRightSuspensionWidget = AddWidget(RearRightSuspension, FText::FromString(TEXT("Rear Right Suspension")), 1, 1);
}

USuspensionWidget* USuspensionHUD::AddWidget(USuspension* Component, const FText& Label, int32 Row, int32 Col)
{
	if (!Component || !Grid) return nullptr;

	USuspensionWidget* Widget = CreateWidget<USuspensionWidget>(GetOwningPlayer(), SuspensionWidgetClass);
	if (!Widget) return nullptr;

	Widget->Setup(Component, Label);
	
	if (UUniformGridSlot* SlotPosition = Grid->AddChildToUniformGrid(Widget, Row, Col))
	{
		SlotPosition->SetHorizontalAlignment(HAlign_Fill);
		SlotPosition->SetVerticalAlignment(VAlign_Fill);
	}

	return Widget;
}
