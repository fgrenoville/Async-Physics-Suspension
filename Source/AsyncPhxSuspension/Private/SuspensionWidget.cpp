// Fill out your copyright notice in the Description page of Project Settings.

#include "SuspensionWidget.h"
#include "Suspension.h"  
#include "Components/Slider.h"
#include "Components/TextBlock.h"

void USuspensionWidget::Setup(USuspension* Component, FText Label)
{
	Suspension = Component;

	if (WidgetName) WidgetName->SetText(Label);
}

void USuspensionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!Suspension.IsValid()) return;

	if (!IsValid(TravelSlider) || !IsValid(SpringStiffnessSlider) || !IsValid(ShockBumpSlider) || !IsValid(ShockReboundSlider)
		|| !IsValid(MaxForceSlider) || !IsValid(WheelRadiusSlider))
		return;
	
	InitSlider(TravelSlider, Suspension->GetTravelCmMinValue(), Suspension->GetTravelCmMaxValue(),
		Suspension->GetTravelCmStepValue(), Suspension->TravelCm);
	
	InitSlider(SpringStiffnessSlider, Suspension->GetSpringStiffnessMinValue(), Suspension->GetSpringStiffnessMaxValue(),
		Suspension->GetSpringStiffnessStepValue(), Suspension->SpringStiffness);
	
	InitSlider(ShockBumpSlider, Suspension->GetShockBumpMinValue(), Suspension->GetShockBumpMaxValue(),
		Suspension->GetShockBumpStepValue(), Suspension->ShockBump);
	
	InitSlider(ShockReboundSlider, Suspension->GetShockReboundMinValue(), Suspension->GetShockReboundMaxValue(),
		Suspension->GetShockReboundStepValue(), Suspension->ShockRebound);
	
	InitSlider(MaxForceSlider, Suspension->GetMaxForceMinValue(), Suspension->GetMaxForceMaxValue(),
		Suspension->GetMaxForceStepValue(), Suspension->MaxForce);
	
	InitSlider(WheelRadiusSlider, Suspension->GetWheelRadiusCmMinValue(), Suspension->GetWheelRadiusCmMaxValue(),
		Suspension->GetWheelRadiusCmStepValue(), Suspension->WheelRadiusCm);

	UpdateText(TravelValue, Suspension->TravelCm);
	UpdateText(SpringStiffnessValue, Suspension->SpringStiffness);
	UpdateText(ShockBumpValue, Suspension->ShockBump);
	UpdateText(ShockReboundValue, Suspension->ShockRebound);
	UpdateText(MaxForceValue, Suspension->MaxForce);
	UpdateText(WheelRadiusValue, Suspension->WheelRadiusCm);
	
	TravelSlider->OnValueChanged.AddDynamic(this, &USuspensionWidget::OnTravelValueChanged);
	SpringStiffnessSlider->OnValueChanged.AddDynamic(this, &USuspensionWidget::OnSpringStiffnessValueChanged);
	ShockBumpSlider->OnValueChanged.AddDynamic(this, &USuspensionWidget::OnShockBumpValueChanged);
	ShockReboundSlider->OnValueChanged.AddDynamic(this, &USuspensionWidget::OnShockReboundValueChanged);
	MaxForceSlider->OnValueChanged.AddDynamic(this, &USuspensionWidget::OnMaxForceValueChanged);
	WheelRadiusSlider->OnValueChanged.AddDynamic(this, &USuspensionWidget::OnWheelRadiusValueChanged);
}

void USuspensionWidget::NativeDestruct()
{
	if (IsValid(TravelSlider)) 
		TravelSlider->OnValueChanged.RemoveDynamic(this, &USuspensionWidget::OnTravelValueChanged);

	if (IsValid(SpringStiffnessSlider)) 
		SpringStiffnessSlider->OnValueChanged.RemoveDynamic(this, &USuspensionWidget::OnSpringStiffnessValueChanged);

	if (IsValid(ShockBumpSlider)) 
		ShockBumpSlider->OnValueChanged.RemoveDynamic(this, &USuspensionWidget::OnShockBumpValueChanged);

	if (IsValid(ShockReboundSlider)) 
		ShockReboundSlider->OnValueChanged.RemoveDynamic(this, &USuspensionWidget::OnShockReboundValueChanged);

	if (IsValid(MaxForceSlider)) 
		MaxForceSlider->OnValueChanged.RemoveDynamic(this, &USuspensionWidget::OnMaxForceValueChanged);

	if (IsValid(WheelRadiusSlider)) 
		WheelRadiusSlider->OnValueChanged.RemoveDynamic(this, &USuspensionWidget::OnWheelRadiusValueChanged);

	Super::NativeDestruct();
}

void USuspensionWidget::InitSlider(USlider* Slider, float Min, float Max, float Step, float StartValue)
{
	if (!Slider) return;
	
	Slider->SetMinValue(Min);
	Slider->SetMaxValue(Max);
	Slider->SetStepSize(Step);
	Slider->SetValue(FMath::Clamp(StartValue, Min, Max));
}

void USuspensionWidget::UpdateText(UTextBlock* Text, float Value, int32 Decimals) const
{
	if (!Text) return;
	
	FNumberFormattingOptions Opt;
	Opt.MinimumFractionalDigits = Decimals;
	Opt.MaximumFractionalDigits = Decimals;
	Text->SetText(FText::AsNumber(Value, &Opt));
}

void USuspensionWidget::OnTravelValueChanged(float NewValue)
{
	ApplySliderChange(Suspension, &USuspension::TravelCm,
					  TravelSlider, TravelValue,
					  NewValue, Suspension->GetTravelCmMinValue(), Suspension->GetTravelCmMaxValue(),
					  Suspension->GetTravelCmStepValue(), 0);
}

void USuspensionWidget::OnSpringStiffnessValueChanged(float NewValue)
{
	ApplySliderChange(Suspension, &USuspension::SpringStiffness,
					  SpringStiffnessSlider, SpringStiffnessValue,
					  NewValue, Suspension->GetSpringStiffnessMinValue(), Suspension->GetSpringStiffnessMaxValue(),
					  Suspension->GetSpringStiffnessStepValue(), 0);
}

void USuspensionWidget::OnShockBumpValueChanged(float NewValue)
{
	ApplySliderChange(Suspension, &USuspension::ShockBump,
					  ShockBumpSlider, ShockBumpValue,
					  NewValue, Suspension->GetShockBumpMinValue(), Suspension->GetShockBumpMaxValue(),
					  Suspension->GetShockBumpStepValue(), 0);
}

void USuspensionWidget::OnShockReboundValueChanged(float NewValue)
{
	ApplySliderChange(Suspension, &USuspension::ShockRebound,
					  ShockReboundSlider, ShockReboundValue,
					  NewValue, Suspension->GetShockReboundMinValue(), Suspension->GetShockReboundMaxValue(),
					  Suspension->GetShockReboundStepValue(), 0);
}

void USuspensionWidget::OnMaxForceValueChanged(float NewValue)
{
	ApplySliderChange(Suspension, &USuspension::MaxForce,
					  MaxForceSlider, MaxForceValue,
					  NewValue, Suspension->GetMaxForceMinValue(), Suspension->GetMaxForceMaxValue(),
					  Suspension->GetMaxForceStepValue(), 0);
}

void USuspensionWidget::OnWheelRadiusValueChanged(float NewValue)
{
	ApplySliderChange(Suspension, &USuspension::WheelRadiusCm,
					  WheelRadiusSlider, WheelRadiusValue,
					  NewValue, Suspension->GetWheelRadiusCmMinValue(), Suspension->GetWheelRadiusCmMaxValue(),
					  Suspension->GetWheelRadiusCmStepValue(), 0);
}
