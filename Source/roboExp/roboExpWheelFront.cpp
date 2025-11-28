// Copyright Epic Games, Inc. All Rights Reserved.

#include "roboExpWheelFront.h"
#include "UObject/ConstructorHelpers.h"

UroboExpWheelFront::UroboExpWheelFront()
{
	AxleType = EAxleType::Front;
	bAffectedBySteering = true;
	MaxSteerAngle = 40.f;
}