// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Slider.h"
#include "SuspensionWidget.generated.h"

class USuspension;
class USlider;
class UTextBlock;

UCLASS(ClassGroup = (AsyncPhxSuspension))
class ASYNCPHXSUSPENSION_API USuspensionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Setup(USuspension* Component, FText Label);
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* WidgetName;

	UPROPERTY(meta = (BindWidget))
	USlider* TravelSlider;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TravelValue;

	UPROPERTY(meta = (BindWidget))
	USlider* SpringStiffnessSlider;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SpringStiffnessValue;

	UPROPERTY(meta = (BindWidget))
	USlider* ShockBumpSlider;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ShockBumpValue;

	UPROPERTY(meta = (BindWidget))
	USlider* ShockReboundSlider;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ShockReboundValue;

	UPROPERTY(meta = (BindWidget))
	USlider* MaxForceSlider;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MaxForceValue;

	UPROPERTY(meta = (BindWidget))
	USlider* WheelRadiusSlider;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* WheelRadiusValue;

private:
	UPROPERTY()
	TWeakObjectPtr<USuspension> Suspension;

private:
	void InitSlider(USlider* Slider, float Min, float Max, float Step, float StartValue);
	void UpdateText(UTextBlock* Text, float Value, int32 Decimals = 0) const;

	template<typename MemberPtr>
	void ApplySliderChange(TWeakObjectPtr<USuspension> S,
						   MemberPtr Field,
						   USlider* Slider, UTextBlock* Text,
						   float NewValue, float Min, float Max, float Step, int32 Decimals = 0)
	{
		if (!S.IsValid() || !Slider || !Text) return;

		const float Clamped = FMath::Clamp(NewValue, Min, Max);
		const float Snapped = (Step > 0.f) ? FMath::RoundToFloat(Clamped / Step) * Step : Clamped;

		S.Get()->*Field = Snapped;

		if (!FMath::IsNearlyEqual(Slider->GetValue(), Snapped))
			Slider->SetValue(Snapped);

		UpdateText(Text, Snapped, Decimals);
	}
	
	UFUNCTION()
	void OnTravelValueChanged(float NewValue);
	UFUNCTION()
	void OnSpringStiffnessValueChanged(float NewValue);
	UFUNCTION()
	void OnShockBumpValueChanged(float NewValue);
	UFUNCTION()
	void OnShockReboundValueChanged(float NewValue);
	UFUNCTION()
	void OnMaxForceValueChanged(float NewValue);
	UFUNCTION()
	void OnWheelRadiusValueChanged(float NewValue);
};
