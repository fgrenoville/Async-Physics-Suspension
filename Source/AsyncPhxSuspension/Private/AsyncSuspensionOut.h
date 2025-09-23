#pragma once

struct FDebugDrawLineCommand
{
	FVector StartPoint = FVector::ZeroVector;
	FVector EndPoint = FVector::ZeroVector;
	FColor Color = FColor::Black;
	float Thickness = 0.0f;
	float Duration = 0.0f;
};

struct FAsyncSuspensionOut
{
	static constexpr int32 MaxDebugPerWheel = 8;
	TArray<FDebugDrawLineCommand, TInlineAllocator<MaxDebugPerWheel>> DebugDrawCommands;
	
	FORCEINLINE void ClearDebugCommandsBuffer()
	{
#if !UE_BUILD_SHIPPING
		{
			DebugDrawCommands.Reset();
		}
#endif
	}

	FORCEINLINE void AddDebugLineCommand(const FVector& StartPoint, const FVector& EndPoint,
		const FColor& Color, const float Thickness, const float Duration)
	{
#if !UE_BUILD_SHIPPING
		if (DebugDrawCommands.Num() >= MaxDebugPerWheel) return;

		DebugDrawCommands.Emplace(FDebugDrawLineCommand{ StartPoint, EndPoint, Color, Thickness, Duration });
#endif
	}
};
