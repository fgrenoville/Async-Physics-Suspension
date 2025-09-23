#pragma once

struct FAsyncSuspensionIn
{
	FTransform AttachLocal = FTransform();

	float TravelCm = 0.0f;
	float SpringStiffness = 0.0f;
	float ShockBump = 0.0f;
	float ShockRebound = 0.0f;
	float MaxForce = 0.0f;
	
	float WheelRadiusCm = 0.0f;
};
